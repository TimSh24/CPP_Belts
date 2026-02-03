/*
На диске есть множество книг — архивированных текстовых файлов. Пользователь запрашивает книгу с заданным названием. 
Надо её считать, распаковать и вернуть. Чаще всего пользователи запрашивают одни и те же книги, поэтому их хочется кэшировать. 
При этом запросы приходят в несколько потоков.
Для доступа к книгам используется интерфейс ICache, объявленный в файле Common.h. У него всего один метод GetBook(), 
который возвращает shared_ptr на книгу (используется синоним BookPtr). Реализация этого метода должна обеспечить правильное кэширование.
Кэширование производится методом вытеснения давно неиспользуемых элементов (Least Recently Used, LRU). Каждый элемент кэша имеет ранг. 
При вызове метода GetBook(), если книга с таким названием уже есть в кэше, её ранг поднимается до максимального (строго больше, чем у всех остальных). 
Если такой книги нет в кэше, то она добавляется в кэш, и её ранг, опять же, выставляется в максимальный. 
При этом, если общий размер книг превышает ограничение max_memory, из кэша удаляются книги с наименьшим рангом, пока это необходимо. 
Возможно, пока он полностью не будет опустошён. Если же размер запрошенной книги уже превышает max_memory, то после вызова метода кэш остаётся пустым, 
то есть книга в него не добавляется.
Метод GetBook() может вызываться одновременно из нескольких потоков.

В рамках задачи были реализованы:
class LruCache : public ICache;
функция unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings& settings);
*/
#include "Common.h"
#include <unordered_map>
#include <map>
#include <atomic>
#include <future>
#include <numeric>
#include <random>
#include <sstream>
#include <mutex>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ): books_unpacker_(move(books_unpacker)),  settings_(move(settings))  {}
    LruCache(LruCache&& other) noexcept{
        bookname_rang = move(other.bookname_rang);
        rang_bookname = move(other.rang_bookname);
        books_ = move(other.books_);
        settings_ = move(other.settings_);
        books_unpacker_ = move(other.books_unpacker_);
        used_memory_ = move(other.used_memory_);
    }

  BookPtr GetBook(const string& book_name) override {
      lock_guard<mutex> g(m);
      if (books_.find(book_name) != books_.end()) {
          auto old_rang = bookname_rang[book_name];
          bookname_rang[book_name] = prev(rang_bookname.end())->first + 1;
          rang_bookname.erase(old_rang);
          rang_bookname[bookname_rang[book_name]] = book_name;
          return books_[book_name];
      }
      else {
          auto unpacked_book = books_unpacker_->UnpackBook(book_name);
          auto current_book_size = unpacked_book->GetContent().size();

          if (settings_.max_memory >= current_book_size) {
              while (settings_.max_memory - used_memory_ < current_book_size) {
                  if (!bookname_rang.empty()) {
                      auto book_with_min_rang = rang_bookname.begin()->second;
                      auto min_rang = rang_bookname.begin()->first;
                      used_memory_ -= books_[book_with_min_rang]->GetContent().size();
                      books_.erase(book_with_min_rang);
                      bookname_rang.erase(book_with_min_rang);
                      rang_bookname.erase(min_rang);
                  }
              }
              books_[book_name] = move(unpacked_book);
              auto max_rang = (bookname_rang.empty()) ? 1 : prev(rang_bookname.end())->first + 1;
              bookname_rang[book_name] = max_rang;
              rang_bookname[max_rang] = book_name;
              used_memory_ += books_[book_name]->GetContent().size();
              return books_[book_name];
          }
          else {
              used_memory_ = 0;
              books_.clear();
              bookname_rang.clear();
              rang_bookname.clear();
          }
      }
      return nullptr;
  }

private:
    map<string, int> bookname_rang = {};
    map<int, string> rang_bookname = {};
    unordered_map<string, BookPtr> books_ = {};
    Settings settings_;
    shared_ptr<IBooksUnpacker> books_unpacker_;
    int used_memory_ = 0;
    mutex m;
};

unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(LruCache(move(books_unpacker), settings));
}



