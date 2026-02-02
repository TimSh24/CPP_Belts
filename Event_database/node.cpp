#include "node.h"

Node::Node(const Comparison cmp) : cmp_(cmp) {}
//bool Node::Evaluate(const Date& date, const string& input_event) =0;



    DateComparisonNode::DateComparisonNode(const Comparison cmp, const Date date) : Node(cmp), date_(date) {}
    bool DateComparisonNode::Evaluate(const Date& input_date, const string& input_event) {
        if (cmp_ == Comparison::Less) {
            return input_date < date_;
        }
        else if (cmp_ == Comparison::LessOrEqual) {
            return input_date <= date_;
        }
        else if (cmp_ == Comparison::Greater) {
            return input_date > date_;
        }
        else if (cmp_ == Comparison::GreaterOrEqual) {
            return input_date >= date_;
        }
        else if (cmp_ == Comparison::Equal) {
            return input_date == date_;
        }
        else if (cmp_ == Comparison::NotEqual) {
            return input_date != date_;
        }
        return true;
    }

    EventComparisonNode::EventComparisonNode (const Comparison cmp, const string event): Node(cmp),
                                                                                    event_(event) {}
    bool EventComparisonNode::Evaluate (const Date& input_date, const string& input_event) {
        if (cmp_ == Comparison::Less) {
            return input_event < event_;
        }
        else if (cmp_ == Comparison::LessOrEqual) {
            return input_event <= event_;
        }
        else if (cmp_ == Comparison::Greater) {
            return input_event > event_;
        }
        else if (cmp_ == Comparison::GreaterOrEqual) {
            return input_event >= event_;
        }
        else if (cmp_ == Comparison::Equal) {
            return input_event == event_;
        }
        else if (cmp_ == Comparison::NotEqual) {
            return input_event != event_;
        }
        return true;
    }

    LogicalOperationNode::LogicalOperationNode(const LogicalOperation op, shared_ptr<Node> left,
                                                                shared_ptr<Node> right) : Node(Comparison::Equal),
                                                                LogOp_(op), left_(left), right_(right) {}
    bool LogicalOperationNode::Evaluate(const Date& date, const string& input_event) {
        auto left_eval = left_->Evaluate(date, input_event);
        auto right_eval = right_->Evaluate(date, input_event);
        if (LogOp_ == LogicalOperation::And) {
            return left_eval && right_eval;
        }
        if (LogOp_ == LogicalOperation::Or) {
            return left_eval || right_eval;
        }
        return true;
    }

    EmptyNode::EmptyNode() : Node(Comparison::Equal) {}
    bool EmptyNode::Evaluate(const Date& date, const string& input_event) {
    return true;
}
