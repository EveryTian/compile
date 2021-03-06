#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <queue>

class Node;
class Program;
class Statement;
class Expression;
class GlobalStatement;
class VariableType;
class GlobalVariableDeclarationStatement;
class VariableDeclarationStatement;
class VariableDeclaration;
class Variable;
class Identifier;
// class GlobalFunctionDeclarationStatement;
class FunctionDeclarationStatement;
class Parameter;
class Block;
class ExpressionStatement;
class ReturnStatement;
class WhileStatement;
class ForStatement;
class IfStatement;
class AssignExpression;
class AndExpression;
class OrExpression;
class GtExpression;
class LtExpression;
class GeExpression;
class LeExpression;
class EqExpression;
class NeExpression;
class AddExpression;
class SubExpression;
class MulExpression;
class DivExpression;
class UnaryExpression;
class NotExpression;
class FunctionCallExpression;
class Argument;
class Value;
class IntValue;
class FloatValue;

// typedef std::vector<GlobalStatement *> GlobalStatementList;
// typedef std::vector<VariableDeclaration *> VariableDeclarationList;
typedef std::vector<Parameter *> ParameterList;
typedef std::vector<VariableDeclarationStatement *> VariableDeclarationStatementList;
typedef std::vector<Statement *> StatementList;
typedef std::vector<Argument *> ArgumentList;

class Node {
public:
    bool isFuncDec = false;
    bool isVarDec = false;
    bool isNeedTypeCheck = false;
    bool isNeedFuncDeclareCheck = false;
    bool isNeedVarDeclareCheck = false;
    bool isUpdateEnv = false;
    bool isRollbackEnv = false;
    int value = 0;
    ParameterList *parameterList = nullptr;
    ArgumentList *argumentList = nullptr;
    VariableType* variableType = nullptr;
    VariableType* type = nullptr;
    Variable *variable = nullptr;
    Identifier *identifier = nullptr;
    Expression *expression1 = nullptr;
    Expression *expression2 = nullptr;
    virtual std::string toString(){
	}
	virtual void pushto(std::queue<Node*> *q){
	}
};

class Program : public Node {
public:
	StatementList *globalStatementList;
	virtual std::string toString() {
        return globalStatementList ? "#Program **glbDecList" : "#Program";
	}
    virtual void pushto(std::queue<Node*> *q){
        if(globalStatementList){
            for(int i = 0; i < globalStatementList->size(); i++){
                q->push((Node*)((*globalStatementList)[i]));
            }
        }
	}
};

class Statement : public Node {
public:
    virtual std::string toString(){
	}
	virtual void pushto(std::queue<Node*> *q){
	}
};

class Expression : public Node {
public:
    virtual std::string toString(){
	}
	virtual void pushto(std::queue<Node*> *q){
	}
};

// class GlobalStatement : Statement {
// public:
// };

class VariableType : public Node {
public:
	enum VariableTypeEnum {
		INT = 1, FLOAT = 2, CHAR = 3, VOID = 4
	};
	VariableTypeEnum variableType;
	VariableType(int type) {
        switch (type) {
        case 1:
            variableType = INT;
            break;
        case 2:
            variableType = FLOAT;
            break;
        case 3:
            variableType = CHAR;
            break;
        case 4:
            variableType = VOID;
            break;
        }
	}
    std::string toString(){
        switch (variableType) {
            case INT:
                return "int";
            case FLOAT:
                return "float";
            case CHAR:
                return "char";
	    case VOID:
                return "void";
        }
	}
	virtual void pushto(std::queue<Node*> *q){

	}
};

// class GlobalVariableDeclarationStatement : GlobalStatement {
// public:
// 	VariableType *variableType;
//     Variable *variable;
//     Expression *expression;
//     GlobalVariableDeclarationStatement(VariableType *varType, Variable *var, Expression *expr) {
//         variableType = varType;
//         variable = var;
//         expression = expr;
//     }
// };

class VariableDeclarationStatement : public Statement {
public:
    Expression *expression;
    VariableDeclarationStatement(VariableType *varType, Variable *var, Expression *expr) {
        isVarDec = true;
        variableType = varType;
        variable = var;
        expression = expr;
        isNeedTypeCheck = true;
        expression1 = (Expression*)var;
        expression2 = expr;
    }
    virtual std::string toString(){
        return "#VarDec *vType *var *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(variableType);
        q->push((Node*)variable);
        q->push(expression);
	}
};

// class VariableDeclaration : Node {
// public:
// 	Variable * variable;
// 	Expression *expression;
// };

class Variable : public Expression {
public:
	Expression *indexExpression;
    Variable(Identifier *ident, Expression *indexExpr) {
    isNeedVarDeclareCheck = true;
        identifier = ident;
        indexExpression = indexExpr;
    }
    virtual std::string toString(){
        if(indexExpression){
            return "#Var *id *expr";
        }
        else{
            return "#Var *id";
        }
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push((Node*)identifier);
        if(indexExpression){
            q->push(indexExpression);
        }
	}
};

class Identifier : public Expression {
public:
	std::string *identifier;
    Identifier(std::string *ident) {
        identifier = ident;
    }
    virtual std::string toString(){
        return *identifier;
	}
	virtual void pushto(std::queue<Node*> *q){

	}
};

// class GlobalFunctionDeclarationStatement : GlobalStatement {
class FunctionDeclarationStatement : public Statement {
public:
	Block *block;
    FunctionDeclarationStatement(VariableType *varType, Identifier *ident, ParameterList *paraList, Block *blk) {
        isFuncDec = true;
        variableType = varType;
        identifier = ident;
        parameterList = paraList;
        block = blk;
    }
    virtual std::string toString(){
        if (parameterList) {
            return "#StmtFuncDec *vType *id **paraList *block";
        } else {
            return "#StmtFuncDec *vType *id *block";
        }
	}
	virtual void pushto(std::queue<Node*> *q){
        if(variableType){
            q->push(variableType);
        }
        q->push(identifier);
        if(parameterList){
            for(int i = 0; i < parameterList->size(); i++){
                q->push((Node*)((*parameterList)[i]));
            }
        }
        if(block){
            q->push((Node*)block);
        }
	}
};

class Parameter : public Node {
public:
    Parameter(VariableType *varType, Identifier *ident) {
        variableType = varType;
	type = varType;
        identifier = ident;
    }
    virtual std::string toString(){
        return "#Para *vType *id";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(variableType);
        q->push((Node*)identifier);
	}
};

class Block : public Statement {
public:
	VariableDeclarationStatementList *variableDeclarationstatementList;
	StatementList *statementList;
    Block(VariableDeclarationStatementList *vdsl, StatementList *sl) {
        variableDeclarationstatementList = vdsl;
        statementList = sl;
    }
    virtual std::string toString(){
        if (variableDeclarationstatementList) {
            return statementList ? "#StmtBlock **varDecStmtList **stmtList" : "#StmtBlock **varDecStmtList";
        }
        else{
            return statementList ? "#StmtBlock **stmtList" : "#StmtBlock";
        }
	}
	virtual void pushto(std::queue<Node*> *q){
        if(variableDeclarationstatementList){
            for(int i = 0; i < variableDeclarationstatementList->size(); i++){
                q->push((Node*)((*variableDeclarationstatementList)[i]));
            }
        }
        for(int i = 0; i < statementList->size(); i++){
            q->push((Node*)((*statementList)[i]));
        }
	}
};

class ExpressionStatement : public Statement {
public:
	Expression *expression;
    ExpressionStatement(Expression *e) {
        expression = e;
    }
    virtual std::string toString(){
        return "#StmtExpr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression);
	}
};

class ReturnStatement : public Statement {
public:
	Expression *expression;
    ReturnStatement(Expression *e) {
        isRollbackEnv = true;
        expression = e;
    }
    virtual std::string toString(){
        return "#StmtReturn *expr";
	}
    virtual void pushto(std::queue<Node*> *q){
        q->push(expression);
	}
};

class WhileStatement : public Statement {
public:
	Expression *expression;
	Statement *statement;
    WhileStatement(Expression *expr, Statement* stmt) {
        expression = expr;
        statement = stmt;
    }
    virtual std::string toString(){
        return "#StmtWhile *expr *stmt";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression);
        q->push(statement);
	}
};

class ForStatement : public Statement {
public:
	Expression *expression1;
	Expression *expression2;
	Expression *expression3;
	Statement *statement;
    ForStatement(Expression *expr1, Expression *expr2, Expression *expr3, Statement *stmt) {
        expression1 = expr1;
        expression2 = expr2;
        expression3 = expr3;
        statement = stmt;
    }
    virtual std::string toString(){
        return "#StmtFor *expr *expr *expr *stmt";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
        q->push(expression3);
        q->push(statement);
	}
};

class IfStatement : public Statement {
public:
	Expression *expression;
	Statement *ifStatement;
	Statement *elseStatement;
    IfStatement(Expression *e, Statement *ts, Statement *es) {
        expression = e;
        ifStatement = ts;
        elseStatement = es;
    }
    virtual std::string toString(){
        if(elseStatement){
            return "#StmtIf *expr *stmt else *stmt";
        }
        else{
            return "#StmtIf *expr *stmt";
        }
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression);
        q->push(ifStatement);
        if(elseStatement){
            q->push(elseStatement);
        }
	}
};

class AssignExpression : public Expression {
public:
    AssignExpression(Variable *v, Expression *e) {
        isNeedTypeCheck = true;
	expression1 = v;
        expression2 = e;
    }
    virtual std::string toString(){
        return "#Assign *var *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class AndExpression : public Expression {
public:
    AndExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#And *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class OrExpression : public Expression {
public:
    OrExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Or *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class GtExpression : public Expression {
public:
    GtExpression(Expression *e1, Expression *e2){
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Gt *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class LtExpression : public Expression {
public:
    LtExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Lt *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class GeExpression : public Expression {
public:
    GeExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Ge *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class LeExpression : public Expression {
public:
    LeExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Le *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class EqExpression : public Expression {
public:
    EqExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Eq *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class NeExpression : public Expression {
public:
    NeExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Ne *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class AddExpression : public Expression {
public:
    AddExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Add *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class SubExpression : public Expression {
public:
    SubExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Sub *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class MulExpression : public Expression {
public:
    MulExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Mul *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class DivExpression : public Expression {
public:
    DivExpression(Expression *e1, Expression *e2) {
        isNeedTypeCheck = true;
        expression1 = e1;
        expression2 = e2;
    }
    virtual std::string toString(){
        return "#Div *expr *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression1);
        q->push(expression2);
	}
};

class UnaryExpression : public Expression {
public:
	Expression * expression;
    UnaryExpression(Expression* e) {
        expression = e;
    }
    virtual std::string toString(){
        return "#Uminus *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression);
	}
};

class NotExpression : public Expression {
public:
	Expression * expression;
    NotExpression(Expression* e) {
        expression = e;
    }
    virtual std::string toString(){
        return "#Not *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression);
	}
};

class FunctionCallExpression : public Expression {
public:
    FunctionCallExpression(Identifier *ident, ArgumentList *argList) {
    isNeedFuncDeclareCheck = true;
        isUpdateEnv = true;
        identifier = ident;
        argumentList = argList;
    }
    virtual std::string toString(){
        if(argumentList){
            return "#Call *id **argList";
        }
        else{
            return "#Call *id";
        }
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(identifier);
        if(argumentList){
            for(int i = 0; i < argumentList->size(); i++){
                q->push((Node*)((*argumentList)[i]));
            }
        }
	}
};

class Argument : public Expression {
public:
	Expression *expression;
    Argument(Expression *expr) {
        expression = expr;
    }
    virtual std::string toString(){
        return "#Arg *expr";
	}
	virtual void pushto(std::queue<Node*> *q){
        q->push(expression);
	}
};

class Value : public Expression {
public:
};

class IntValue : public Value {
public:
	IntValue(int v) { value = v; type = new VariableType(1); }
	virtual std::string toString(){
        return std::to_string(value);
	}
	virtual void pushto(std::queue<Node*> *q){

	}
};

class FloatValue : public Value {
public:
	float value;
	FloatValue(float v) { value = v;type = new VariableType(2); }
	virtual std::string toString(){
        return std::to_string(value);
	}
	virtual void pushto(std::queue<Node*> *q){
	}
};

#endif
