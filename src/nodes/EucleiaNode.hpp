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

class BaseNode
{
public:
    using SharedNode = std::shared_ptr<BaseNode>;

    BaseNode() = default;
    virtual ~BaseNode() = default;

    enum NodeType // TODO: - remove this crap and use typeid()
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


    template <class TNode>
    const TNode &castNode() const
    {
        return static_cast<TNode &>(*this);
    }


    template <class TNode>
    TNode &castNode()
    {
        return static_cast<TNode &>(*this);
    }


    virtual inline NodeType type() const // TODO: - remove.
    {
        return NodeType::None;
    }

    virtual BaseObject *evaluate(Scope &scope) = 0;

    template <typename T>
    T *evaluate(Scope &scope)
    {
        return static_cast<T *>(evaluate(scope));
    }

    // TODO: - add cast methods here.
};

class AddBoolNode : public BaseNode
{
public:
    AddBoolNode(bool state_) : boolObject(state_) {}

    inline NodeType type() const override
    {
        return NodeType::Bool;
    }

    // Creates a BoolObject in the scope and returns managed pointer to it.
    BoolObject *evaluate(Scope &scope) override;

    // protected:
    BoolObject boolObject;
};


class AddIntNode : public BaseNode
{
public:
    AddIntNode(long int_) : intObject(int_) {}

    inline NodeType type() const override
    {
        return NodeType::Int;
    }

    // Creates an IntObject in the scope and returns a managed pointer to it.
    IntObject *evaluate(Scope &scope) override;

    // protected:
    IntObject intObject;
};


class AddFloatNode : public BaseNode
{
public:
    AddFloatNode(double float_) : floatObject(float_) {}

    inline NodeType type() const override
    {
        return NodeType::Float;
    }

    // Returns a FloatObject in the current scope and returns a managed pointer.
    FloatObject *evaluate(Scope &scope) override;

    // protected:
    FloatObject floatObject;
};

class AddStringNode : public BaseNode
{
public:
    AddStringNode(std::string string_) : stringObject(std::move(string_)) {}

    inline NodeType type() const override
    {
        return NodeType::String;
    }

    // Creates a StringObject in the scope and returns managed pointer to it.
    StringObject *evaluate(Scope &scope) override;

    // protected:
    StringObject stringObject;
};


class LookupVariableNode : public BaseNode
{
public:
    LookupVariableNode(std::string variableName_) : variableName(std::move(variableName_)) {}

    inline NodeType type() const override
    {
        return NodeType::VariableName;
    }

    // Returns the object in the scope associated with a variable name.
    BaseObject *evaluate(Scope &scope) override;

    // protected:
    std::string variableName;
};


class AddNewVariableNode : public LookupVariableNode
{
public:
    enum VariableType // TODO: - don't use an enum. Somehow use a typeid to pass -in the desired object type to create.
    {
        Int,
        Float,
        Bool,
        String,
        Array
        // Function	<-- allow this to be set.
    };

    std::string description() const;

    AddNewVariableNode(std::string variableName_, VariableType variableType_)
        : LookupVariableNode(std::move(variableName_)),
          variableType(variableType_) {}

    inline NodeType type() const override
    {
        return NodeType::Variable;
    }

    // Creates a new empty variable of a given type to the scope (i.e. int a;).
    BaseObject *evaluate(Scope &scope) override;

    // protected:
    //  Type checking for variable assignment.
    bool passesAssignmentTypeCheck(const BaseObject &assignObject) const;

    const VariableType variableType;
};


class ProgramNode : public BaseNode
{
public:
    ProgramNode(std::vector<SharedNode> nodes_) : programNodes(std::move(nodes_)) {}

    inline NodeType type() const override
    {
        return NodeType::Program;
    }

    // Evaluates a vector of nodes sequentially. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;

    // protected:
    std::vector<std::shared_ptr<BaseNode>> programNodes; // TODO: - should be vector of pointers.
};


class AddArrayNode : public ProgramNode
{
public:
    AddArrayNode(std::vector<SharedNode> nodes_) : ProgramNode(std::move(nodes_)) {}

    // Create a new ArrayObject in the current scope and return a managed pointer to it.
    ArrayObject *evaluate(Scope &scope) override;

    inline NodeType type() const override
    {
        return NodeType::Array;
    }
};


class FileNode : public ProgramNode
{
public:
    FileNode(std::vector<SharedNode> nodes_) : ProgramNode(std::move(nodes_)) {}

    // Evaluates a file treating it as one large program. Does NOT create an inner
    // scope node in order to ensure that any functions declared in this file will
    // be added to the master file by using the same global scope - TODO: - think about this logic.
    BaseObject *evaluate(Scope &globalScope) override;

    inline NodeType type() const override
    {
        return NodeType::File;
    }
};

// TODO: - rewrite this...
class ArrayAccessNode : public BaseNode
{
public:
    ArrayAccessNode(std::shared_ptr<BaseNode> _arrayName, std::shared_ptr<BaseNode> _arrayIndex)
        : arrayName(std::static_pointer_cast<AddNewVariableNode>(_arrayName)),
          arrayIndex(std::static_pointer_cast<AddIntNode>(_arrayIndex))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::ArrayAccess;
    }

    BaseObject *evaluate(Scope &scope) override;

    std::shared_ptr<AddNewVariableNode> arrayName{nullptr}; // TODO: - don't use shared pointers.
    std::shared_ptr<AddIntNode> arrayIndex{nullptr};
};


class IfNode : public BaseNode
{
public:
    IfNode(std::shared_ptr<BaseNode> condition_,
           std::shared_ptr<BaseNode> thenDo_,
           std::shared_ptr<BaseNode> elseDo_ = nullptr)
        : ifCondition(condition_),
          thenDo(thenDo_),
          elseDo(elseDo_)
    {
    }

    inline NodeType type() const override
    {
        return NodeType::If;
    }

    // Evaluate an if/else statement in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;

    // protected:
    std::shared_ptr<BaseNode> ifCondition{nullptr};
    std::shared_ptr<BaseNode> thenDo{nullptr};
    std::shared_ptr<BaseNode> elseDo{nullptr}; // Links to next argument (if node!).
};


class DoWhileNode : public BaseNode
{
public:
    DoWhileNode(SharedNode condition_, SharedNode body_)
        : condition(std::move(condition_)),
          body(std::move(body_))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::DoWhile;
    }

    // Evaluate a do-while loop in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;

    // protected:
    std::shared_ptr<BaseNode> condition{nullptr};
    std::shared_ptr<BaseNode> body{nullptr};
};


class WhileNode : public DoWhileNode
{
public:
    WhileNode(SharedNode _condition, SharedNode _body)
        : DoWhileNode(std::move(_condition), std::move(_body))
    {
    }

    // Evaluate a while loop in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;

    inline NodeType type() const override
    {
        return NodeType::While;
    }
};


class ForLoopNode : public BaseNode
{
public:
    ForLoopNode(SharedNode start_, SharedNode condition_, SharedNode update_, SharedNode body_)
        : start(std::move(start_)),
          condition(std::move(condition_)),
          update(std::move(update_)),
          body(std::move(body_))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::ForLoop;
    }

    // Evaluates a for-loop in current scope. Returns nullptr.
    BaseObject *evaluate(Scope &scope) override;

    // protected:
    std::shared_ptr<BaseNode> start{nullptr};
    std::shared_ptr<BaseNode> condition{nullptr};
    std::shared_ptr<BaseNode> update{nullptr};
    std::shared_ptr<BaseNode> body{nullptr};
};


class FunctionCallNode : public BaseNode
{
public:
    FunctionCallNode(SharedNode funcName_, SharedNode funcArgs_)
        : funcName(std::static_pointer_cast<AddNewVariableNode>(funcName_)),
          funcArgs(std::static_pointer_cast<ProgramNode>(funcArgs_))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::FunctionCall;
    }

    // TODO: - don't forget to do performance profiling for Fib sequence and see memory requirements for old and new version
    // TODO: - create a new PR after this for parser to store all nodes in AST in flat array using pointers with method to delete by walking along array.
    BaseObject *evaluate(Scope &scope) override;

    BaseObject *evaluateFunctionBody(BaseNode &funcBody, Scope &funcScope);

    // protected:
    std::shared_ptr<AddNewVariableNode> funcName{nullptr};
    std::shared_ptr<ProgramNode> funcArgs{nullptr};
};


class FunctionNode : public FunctionCallNode, public std::enable_shared_from_this<FunctionNode>
{
public:
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

    BaseObject *evaluate(Scope &scope) override;

    std::shared_ptr<ProgramNode> funcBody{nullptr};
};


class BreakNode : public BaseNode
{
    inline NodeType type() const override
    {
        return NodeType::Break;
    }

    BaseObject *evaluate(Scope &scope) override;
};


class ReturnNode : public BaseNode
{
public:
    ReturnNode(SharedNode _returnValue = nullptr)
        : returnNode(std::move(_returnValue))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Return;
    }

    BaseObject *evaluate(Scope &scope) override;

    SharedNode returnNode{nullptr};
};


class AssignNode : public BaseNode
{
public:
    AssignNode(SharedNode _left, SharedNode _right)
        : left(std::move(_left)),
          right(std::move(_right))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Assignment;
    }

    BaseObject *evaluate(Scope &scope) override;

    BaseObject *evaluateArrayAccess(Scope &scope);

    SharedNode left{nullptr};
    SharedNode right{nullptr};
};


class BinaryNode : public AssignNode
{
public:
    BinaryNode(SharedNode left_, SharedNode right_, std::string binaryOperator_)
        : AssignNode(std::move(left_), std::move(right_)),
          binaryOperator(std::move(binaryOperator_))
    {
    }

    inline NodeType type() const override
    {
        return NodeType::Binary;
    }

    BaseObject *evaluate(Scope &scope) override;

    std::string binaryOperator;

    // protected:
    BaseObject *applyOperator(Scope &scope, const BaseObject &left, const BaseObject &right) const;

    BaseObject *applyOperator(Scope &scope, const IntObject &left, const IntObject &right) const;
    BaseObject *applyOperator(Scope &scope, const FloatObject &left, const FloatObject &right) const;
    BaseObject *applyOperator(Scope &scope, const StringObject &left, const StringObject &right) const;
};


#endif /* EucleiaNode_hpp */
