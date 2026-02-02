#pragma once
#include <iostream>
#include "date.h"
#include <sstream>
#include <memory>

using namespace std;

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation {
    And,
    Or
};

class Node {
public:
    Node (const Comparison cmp);
    virtual bool Evaluate(const Date& date, const string& input_event) = 0;
protected:
    Comparison cmp_;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison cmp, const Date date);
    bool Evaluate(const Date& input_date, const string& input_event) override;

private:
    const Date date_;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode (const Comparison cmp, const string event);
    bool Evaluate (const Date& input_date, const string& input_event) override;

private:
    const string event_;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode (const LogicalOperation op, shared_ptr<Node> left, shared_ptr<Node> right);
    bool Evaluate(const Date& date, const string& input_event) override;

private:
    LogicalOperation LogOp_;
    shared_ptr<Node> left_;
    shared_ptr<Node> right_;
};

class EmptyNode : public Node {
public:
    EmptyNode();
    bool Evaluate(const Date& date, const string& input_event) override;
};
