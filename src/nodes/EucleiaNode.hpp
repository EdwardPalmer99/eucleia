//
//  EucleiaNode.hpp
//  Eucleia
//
//  Created by Edward on 14/01/2024.
//

#ifndef EuclieaNode_hpp
#define EuclieaNode_hpp

#include "EucleiaObject.hpp"
#include "EucleiaScope.hpp"
#include <memory>
#include <string>

struct BaseNode
{
    typedef std::shared_ptr<BaseNode> SharedNode;

    BaseNode() = default;
    virtual ~BaseNode() = default;

    enum NodeType
    {
        None,
        Binary,
        Assignment,
        Program,
        Array,
        ArrayAccess,
        If,
        While,
        DoWhile,
        ForLoop,
        Function,
        FunctionCall,
        Float,
        Int,
        Bool,
        String,
        Variable,
        VariableName,
        Break,
        Return,
        Not,
        PrefixIncrement,
        PrefixDecrement,
        Negation,
        File,
        Library
    };

    virtual inline NodeType type() const
    {
        return NodeType::None;
    }

    virtual std::shared_ptr<BaseObject> evaluate(Scope &scope) = 0;

    template <typename T>
    std::shared_ptr<T> evaluate(Scope &scope)
    {
        return std::static_pointer_cast<T>(evaluate(scope));
    }
};


struct BoolNode : BaseNode
{
    BoolNode(bool state)
        : boolValue{state}
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Bool;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    const bool boolValue{false};
};


struct StringNode : BaseNode
{
    StringNode(std::string &string)
        : stringValue{string}
    {
    }

    inline NodeType type() const override
    {
        return NodeType::String;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    const std::string stringValue;
};


/// This is for variables that have already been declared ( varName = 2; )
struct VariableNameNode : BaseNode
{
    VariableNameNode(std::string &name)
        : variableName{name}
    {
    }

    inline NodeType type() const override
    {
        return NodeType::VariableName;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    const std::string variableName;
};


/// This is for variables that are being declared i.e. ( varName : int = 2; );
struct VariableNode : VariableNameNode
{
    enum Type
    {
        Int,
        Float,
        Bool,
        String,
        Array
        // Function	<-- allow this to be set.
    };

    std::string description() const;

    VariableNode(std::string &name, Type _type)
        : VariableNameNode(name),
          variableType{_type}
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Variable;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    // Type checking for variable assignment.
    bool passesAssignmentTypeCheck(const BaseObject &assignObject) const;

    const Type variableType;
};


struct IntNode : BaseNode
{
    IntNode(long number)
        : numberValue{number}
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Int;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    const long numberValue;
};


struct FloatNode : BaseNode
{
    FloatNode(double number)
        : numberValue{number}
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Float;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    const double numberValue;
};


struct ProgramNode : BaseNode
{
    ProgramNode(std::vector<SharedNode> _nodes)
        : nodes(std::move(_nodes))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Program;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::vector<std::shared_ptr<BaseNode>> nodes;
};


/// A program node for an entire file.
struct FileNode : ProgramNode
{
    FileNode(std::vector<SharedNode> _nodes)
        : ProgramNode(std::move(_nodes))
    {
    }

    std::shared_ptr<BaseObject> evaluate(Scope &globalScope) override;

    inline NodeType type() const override
    {
        return NodeType::File;
    }
};


struct ArrayNode : ProgramNode
{
    ArrayNode(std::vector<SharedNode> _nodes)
        : ProgramNode(_nodes)
    {
    }
    ArrayNode(ProgramNode &_program)
        : ProgramNode(std::move(_program))
    {
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    inline NodeType type() const override
    {
        return NodeType::Array;
    }
};


struct ArrayAccessNode : BaseNode
{
    ArrayAccessNode(std::shared_ptr<BaseNode> _arrayName, std::shared_ptr<BaseNode> _arrayIndex)
        : arrayName(std::static_pointer_cast<VariableNode>(_arrayName)),
          arrayIndex(std::static_pointer_cast<IntNode>(_arrayIndex))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::ArrayAccess;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::shared_ptr<VariableNode> arrayName{nullptr};
    std::shared_ptr<IntNode> arrayIndex{nullptr};
};


struct IfNode : BaseNode
{
    IfNode(std::shared_ptr<BaseNode> _condition,
           std::shared_ptr<BaseNode> _thenDo,
           std::shared_ptr<BaseNode> _elseDo = nullptr)
        : ifCondition{_condition},
          thenDo{_thenDo},
          elseDo{_elseDo}
    {
    }

    inline NodeType type() const override
    {
        return NodeType::If;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::shared_ptr<BaseNode> ifCondition{nullptr};
    std::shared_ptr<BaseNode> thenDo{nullptr};
    std::shared_ptr<BaseNode> elseDo{nullptr}; // Links to next argument (if node!).
};


struct DoWhileNode : BaseNode
{
    DoWhileNode(SharedNode _condition, SharedNode _body)
        : condition(std::move(_condition)),
          body(std::move(_body))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::DoWhile;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::shared_ptr<BaseNode> condition{nullptr};
    std::shared_ptr<BaseNode> body{nullptr};
};


struct WhileNode : DoWhileNode
{
    WhileNode(SharedNode _condition, SharedNode _body)
        : DoWhileNode(std::move(_condition), std::move(_body))
    {
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    inline NodeType type() const override
    {
        return NodeType::While;
    }
};


struct ForLoopNode : BaseNode
{
    ForLoopNode(SharedNode _start, SharedNode _condition, SharedNode _update, SharedNode _body)
        : start(std::move(_start)),
          condition(std::move(_condition)),
          update(std::move(_update)),
          body(std::move(_body))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::ForLoop;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::shared_ptr<BaseNode> start{nullptr};
    std::shared_ptr<BaseNode> condition{nullptr};
    std::shared_ptr<BaseNode> update{nullptr};
    std::shared_ptr<BaseNode> body{nullptr};
};


struct FunctionCallNode : BaseNode
{
    FunctionCallNode(SharedNode _funcName, SharedNode _funcArgs)
        : funcName(std::static_pointer_cast<VariableNode>(_funcName)),
          funcArgs(std::static_pointer_cast<ProgramNode>(_funcArgs))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::FunctionCall;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::shared_ptr<BaseObject> evaluateFunctionBody(BaseNode &funcBody, Scope &funcScope);

    std::shared_ptr<VariableNode> funcName{nullptr};
    std::shared_ptr<ProgramNode> funcArgs{nullptr};
};


struct FunctionNode : FunctionCallNode, public std::enable_shared_from_this<FunctionNode>
{
    FunctionNode(SharedNode _funcName, SharedNode _funcArgs, SharedNode _funcBody)
        : FunctionCallNode(std::move(_funcName), std::move(_funcArgs)),
          funcBody(std::static_pointer_cast<ProgramNode>(_funcBody))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Function;
    }

    std::shared_ptr<FunctionNode> getShared()
    {
        return shared_from_this();
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::shared_ptr<ProgramNode> funcBody{nullptr};
};


struct BreakNode : BaseNode
{
    inline NodeType type() const override
    {
        return NodeType::Break;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;
};


struct ReturnNode : BaseNode
{
    ReturnNode(SharedNode _returnValue = nullptr)
        : returnNode(std::move(_returnValue))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Return;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    SharedNode returnNode{nullptr};
};


struct AssignNode : BaseNode
{
    AssignNode(SharedNode _left, SharedNode _right)
        : left(std::move(_left)),
          right(std::move(_right))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Assignment;
    }

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::shared_ptr<BaseObject> evaluateArrayAccess(Scope &scope);

    SharedNode left{nullptr};
    SharedNode right{nullptr};
};


struct BinaryNode : AssignNode
{
    BinaryNode(SharedNode left_, SharedNode right_, std::string binaryOperator_)
        : AssignNode(std::move(left_), std::move(right_)),
          binaryOperator(std::move(binaryOperator_))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Binary;
    }

    std::shared_ptr<BaseObject> applyOperator(std::shared_ptr<BaseObject> left, std::shared_ptr<BaseObject> right);

    IntObject applyOperator(const IntObject &left, const IntObject &right) const;
    FloatObject applyOperator(const FloatObject &left, const FloatObject &right) const;

    std::shared_ptr<BaseObject> evaluate(Scope &scope) override;

    std::string binaryOperator;
};


#endif /* EucleiaNode_hpp */
