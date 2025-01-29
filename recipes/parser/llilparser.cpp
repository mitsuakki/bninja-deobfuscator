#include "llilparser.h"

#include <cstdio>
#include <vector>
#include <string>

using namespace BinaryNinja;
using namespace std;

void PrintIndent(const size_t indent) {
    for (size_t i = 0; i < indent; i++)
        LogInfo("    ");
}

#define ENUM_PRINTER(op) \
    case op: \
        LogInfo(#op); \
        break;

void PrintOperation(const BNLowLevelILOperation operation) {
    switch (operation) {
        ENUM_PRINTER(LLIL_NOP)
        ENUM_PRINTER(LLIL_SET_REG)
        ENUM_PRINTER(LLIL_SET_REG_SPLIT)
        ENUM_PRINTER(LLIL_SET_FLAG)
        ENUM_PRINTER(LLIL_LOAD)
        ENUM_PRINTER(LLIL_STORE)
        ENUM_PRINTER(LLIL_PUSH)
        ENUM_PRINTER(LLIL_POP)
        ENUM_PRINTER(LLIL_REG)
        ENUM_PRINTER(LLIL_CONST)
        ENUM_PRINTER(LLIL_CONST_PTR)
        ENUM_PRINTER(LLIL_EXTERN_PTR)
        ENUM_PRINTER(LLIL_FLAG)
        ENUM_PRINTER(LLIL_FLAG_BIT)
        ENUM_PRINTER(LLIL_ADD)
        ENUM_PRINTER(LLIL_ADC)
        ENUM_PRINTER(LLIL_SUB)
        ENUM_PRINTER(LLIL_SBB)
        ENUM_PRINTER(LLIL_AND)
        ENUM_PRINTER(LLIL_OR)
        ENUM_PRINTER(LLIL_XOR)
        ENUM_PRINTER(LLIL_LSL)
        ENUM_PRINTER(LLIL_LSR)
        ENUM_PRINTER(LLIL_ASR)
        ENUM_PRINTER(LLIL_ROL)
        ENUM_PRINTER(LLIL_RLC)
        ENUM_PRINTER(LLIL_ROR)
        ENUM_PRINTER(LLIL_RRC)
        ENUM_PRINTER(LLIL_MUL)
        ENUM_PRINTER(LLIL_MULU_DP)
        ENUM_PRINTER(LLIL_MULS_DP)
        ENUM_PRINTER(LLIL_DIVU)
        ENUM_PRINTER(LLIL_DIVU_DP)
        ENUM_PRINTER(LLIL_DIVS)
        ENUM_PRINTER(LLIL_DIVS_DP)
        ENUM_PRINTER(LLIL_MODU)
        ENUM_PRINTER(LLIL_MODU_DP)
        ENUM_PRINTER(LLIL_MODS)
        ENUM_PRINTER(LLIL_MODS_DP)
        ENUM_PRINTER(LLIL_NEG)
        ENUM_PRINTER(LLIL_NOT)
        ENUM_PRINTER(LLIL_SX)
        ENUM_PRINTER(LLIL_ZX)
        ENUM_PRINTER(LLIL_LOW_PART)
        ENUM_PRINTER(LLIL_JUMP)
        ENUM_PRINTER(LLIL_JUMP_TO)
        ENUM_PRINTER(LLIL_CALL)
        ENUM_PRINTER(LLIL_CALL_STACK_ADJUST)
        ENUM_PRINTER(LLIL_TAILCALL)
        ENUM_PRINTER(LLIL_RET)
        ENUM_PRINTER(LLIL_NORET)
        ENUM_PRINTER(LLIL_IF)
        ENUM_PRINTER(LLIL_GOTO)
        ENUM_PRINTER(LLIL_FLAG_COND)
        ENUM_PRINTER(LLIL_CMP_E)
        ENUM_PRINTER(LLIL_CMP_NE)
        ENUM_PRINTER(LLIL_CMP_SLT)
        ENUM_PRINTER(LLIL_CMP_ULT)
        ENUM_PRINTER(LLIL_CMP_SLE)
        ENUM_PRINTER(LLIL_CMP_ULE)
        ENUM_PRINTER(LLIL_CMP_SGE)
        ENUM_PRINTER(LLIL_CMP_UGE)
        ENUM_PRINTER(LLIL_CMP_SGT)
        ENUM_PRINTER(LLIL_CMP_UGT)
        ENUM_PRINTER(LLIL_TEST_BIT)
        ENUM_PRINTER(LLIL_BOOL_TO_INT)
        ENUM_PRINTER(LLIL_ADD_OVERFLOW)
        ENUM_PRINTER(LLIL_SYSCALL)
        ENUM_PRINTER(LLIL_BP)
        ENUM_PRINTER(LLIL_TRAP)
        ENUM_PRINTER(LLIL_UNDEF)
        ENUM_PRINTER(LLIL_UNIMPL)
        ENUM_PRINTER(LLIL_UNIMPL_MEM)
        ENUM_PRINTER(LLIL_SET_REG_SSA)
        ENUM_PRINTER(LLIL_SET_REG_SSA_PARTIAL)
        ENUM_PRINTER(LLIL_SET_REG_SPLIT_SSA)
        ENUM_PRINTER(LLIL_REG_SPLIT_DEST_SSA)
        ENUM_PRINTER(LLIL_REG_SSA)
        ENUM_PRINTER(LLIL_REG_SSA_PARTIAL)
        ENUM_PRINTER(LLIL_SET_FLAG_SSA)
        ENUM_PRINTER(LLIL_FLAG_SSA)
        ENUM_PRINTER(LLIL_FLAG_BIT_SSA)
        ENUM_PRINTER(LLIL_CALL_SSA)
        ENUM_PRINTER(LLIL_SYSCALL_SSA)
        ENUM_PRINTER(LLIL_TAILCALL_SSA)
        default:
            LogInfo("<invalid operation %" PRId32 ">", operation);
            break;
    }
}

void LogInfolagCondition(const BNLowLevelILFlagCondition cond) {
    switch (cond) {
        ENUM_PRINTER(LLFC_E)
        ENUM_PRINTER(LLFC_NE)
        ENUM_PRINTER(LLFC_SLT)
        ENUM_PRINTER(LLFC_ULT)
        ENUM_PRINTER(LLFC_SLE)
        ENUM_PRINTER(LLFC_ULE)
        ENUM_PRINTER(LLFC_SGE)
        ENUM_PRINTER(LLFC_UGE)
        ENUM_PRINTER(LLFC_SGT)
        ENUM_PRINTER(LLFC_UGT)
        ENUM_PRINTER(LLFC_NEG)
        ENUM_PRINTER(LLFC_POS)
        ENUM_PRINTER(LLFC_O)
        ENUM_PRINTER(LLFC_NO)
        default:
            LogInfo("<invalid condition>");
            break;
    }
}

void PrintRegister(const LowLevelILFunction* func, const uint32_t reg) {
    if (LLIL_REG_IS_TEMP(reg))
        LogInfo("temp%d", LLIL_GET_TEMP_REG_INDEX(reg));
    else {
        const string name = func->GetArchitecture()->GetRegisterName(reg);
        if (name.empty())
            LogInfo("<no name>");
        else
            LogInfo("%s", name.c_str());
    }
}

void LogInfolag(const LowLevelILFunction* func, const uint32_t flag) {
    if (LLIL_REG_IS_TEMP(flag))
        LogInfo("cond:%d", LLIL_GET_TEMP_REG_INDEX(flag));
    else {
        const string name = func->GetArchitecture()->GetFlagName(flag);
        if (name.empty())
            LogInfo("<no name>");
        else
            LogInfo("%s", name.c_str());
    }
}

void PrintILExpr(const LowLevelILInstruction& instr, size_t indent) {
    PrintIndent(indent);
    PrintOperation(instr.operation);
    LogInfo("\n");

    indent++;

    for (auto& operand : instr.GetOperands()) {
        switch (operand.GetType()) {
            case IntegerLowLevelOperand:
                PrintIndent(indent);
                LogInfo("int 0x%" PRIx64 "\n", operand.GetInteger());
                break;

            case IndexLowLevelOperand:
                PrintIndent(indent);
                LogInfo("index %" PRIdPTR "\n", operand.GetIndex());
                break;

            case ExprLowLevelOperand:
                PrintILExpr(operand.GetExpr(), indent);
                break;

            case RegisterLowLevelOperand:
                PrintIndent(indent);
                LogInfo("reg ");
                PrintRegister(instr.function, operand.GetRegister());
                LogInfo("\n");
                break;

            case FlagLowLevelOperand:
                PrintIndent(indent);
                LogInfo("flag ");
                LogInfolag(instr.function, operand.GetFlag());
                LogInfo("\n");
                break;

            case FlagConditionLowLevelOperand:
                PrintIndent(indent);
                LogInfo("flag condition ");
                LogInfolagCondition(operand.GetFlagCondition());
                LogInfo("\n");
                break;

            case SSARegisterLowLevelOperand:
                PrintIndent(indent);
                LogInfo("ssa reg ");
                PrintRegister(instr.function, operand.GetSSARegister().reg);
                LogInfo("#%" PRIdPTR "\n", operand.GetSSARegister().version);
                break;

            case SSAFlagLowLevelOperand:
                PrintIndent(indent);
                LogInfo("ssa flag ");
                LogInfolag(instr.function, operand.GetSSAFlag().flag);
                LogInfo("#%" PRIdPTR " ", operand.GetSSAFlag().version);
                break;

            case IndexListLowLevelOperand:
                PrintIndent(indent);
                LogInfo("index list ");
                for (auto i : operand.GetIndexList())
                    LogInfo("%" PRIdPTR " ", i);
                LogInfo("\n");
                break;

            case SSARegisterListLowLevelOperand:
                PrintIndent(indent);
                LogInfo("ssa reg list ");
                for (auto i : operand.GetSSARegisterList()) {
                    PrintRegister(instr.function, i.reg);
                    LogInfo("#%" PRIdPTR " ", i.version);
                }
                LogInfo("\n");
                break;

            case SSAFlagListLowLevelOperand:
                PrintIndent(indent);
                LogInfo("ssa reg list ");
                for (auto i : operand.GetSSAFlagList()) {
                    LogInfolag(instr.function, i.flag);
                    LogInfo("#%" PRIdPTR " ", i.version);
                }
                LogInfo("\n");
                break;

            default:
                PrintIndent(indent);
                LogInfo("<invalid operand>\n");
                break;
        }
    }
}

void AnalyzeLLILFunction(const Ref<Function>& func) {
    LogInfo("\n"); // prettier

    // Get the name of the function and display it
    Ref<Symbol> sym = func->GetSymbol();
    if (sym)
        LogInfo("Function %s:\n", sym->GetFullName().c_str());
    else
        LogInfo("Function at 0x%" PRIx64 ":\n", func->GetStart());

    // Fetch the low level IL for the function
    Ref<LowLevelILFunction> il = func->GetLowLevelIL();
    if (!il) {
        LogInfo("    Does not have LLIL\n\n");
        return;
    }

    // Loop through all blocks in the function
    for (auto& block : il->GetBasicBlocks()) {
        // Loop though each instruction in the block
        for (size_t instrIndex = block->GetStart(); instrIndex < block->GetEnd(); instrIndex++) {
            // Fetch IL instruction
            LowLevelILInstruction instr = (*il)[instrIndex];

            // Display core's interpretation of the IL instruction
            vector<InstructionTextToken> tokens;
            il->GetInstructionText(func, func->GetArchitecture(), instrIndex, tokens);
            LogInfo("    %" PRIdPTR " @ 0x%" PRIx64 "  ", instrIndex, instr.address);
            for (auto& token : tokens)
                LogInfo("%s", token.text.c_str());
            LogInfo("\n");

            // Generically parse the IL tree and display the parts
            PrintILExpr(instr, 2);

            // Example of using visitors to find all constants in the instruction
            instr.VisitExprs([&](const LowLevelILInstruction& expr) {
                switch (expr.operation) {
                    case LLIL_CONST:
                    case LLIL_CONST_PTR:
                    case LLIL_EXTERN_PTR:
                        LogInfo("        Found constant 0x%" PRIx64 "\n", expr.GetConstant());
                        return false;  // Done parsing this
                    default:
                        break;
                }
                return true;  // Parse any subexpressions
            });

            // Example of using the templated accessors for efficiently parsing load instructions
            instr.VisitExprs([&](const LowLevelILInstruction& expr) {
                switch (expr.operation) {
                    case LLIL_LOAD:
                        if (expr.GetSourceExpr<LLIL_LOAD>().operation == LLIL_CONST_PTR) {
                            LogInfo("        Loading from address 0x%" PRIx64 "\n",
                                expr.GetSourceExpr<LLIL_LOAD>().GetConstant<LLIL_CONST_PTR>());
                            return false;  // Done parsing this
                        }
                        else if (expr.GetSourceExpr<LLIL_LOAD>().operation == LLIL_EXTERN_PTR) {
                            LogInfo("        Loading from address 0x%" PRIx64 "\n",
                                expr.GetSourceExpr<LLIL_LOAD>().GetConstant<LLIL_EXTERN_PTR>());
                            return false;  // Done parsing this
                        }
                        break;
                    default:
                        break;
                }
                return true;  // Parse any subexpressions
            });
        }
    }

    LogInfo("\n");
}