#include "mlilparser.h"
#include "llilparser.h"

using namespace BinaryNinja;
using namespace std;

#define ENUM_PRINTER(op) \
	case op: \
		LogInfo(#op); \
		break;

void PrintMLILOperation(BNMediumLevelILOperation operation)
{
	switch (operation)
	{
		ENUM_PRINTER(MLIL_NOP)
		ENUM_PRINTER(MLIL_SET_VAR)
		ENUM_PRINTER(MLIL_SET_VAR_FIELD)
		ENUM_PRINTER(MLIL_SET_VAR_SPLIT)
		ENUM_PRINTER(MLIL_LOAD)
		ENUM_PRINTER(MLIL_LOAD_STRUCT)
		ENUM_PRINTER(MLIL_STORE)
		ENUM_PRINTER(MLIL_STORE_STRUCT)
		ENUM_PRINTER(MLIL_VAR)
		ENUM_PRINTER(MLIL_VAR_FIELD)
		ENUM_PRINTER(MLIL_ADDRESS_OF)
		ENUM_PRINTER(MLIL_ADDRESS_OF_FIELD)
		ENUM_PRINTER(MLIL_CONST)
		ENUM_PRINTER(MLIL_CONST_DATA)
		ENUM_PRINTER(MLIL_CONST_PTR)
		ENUM_PRINTER(MLIL_EXTERN_PTR)
		ENUM_PRINTER(MLIL_ADD)
		ENUM_PRINTER(MLIL_ADC)
		ENUM_PRINTER(MLIL_SUB)
		ENUM_PRINTER(MLIL_SBB)
		ENUM_PRINTER(MLIL_AND)
		ENUM_PRINTER(MLIL_OR)
		ENUM_PRINTER(MLIL_XOR)
		ENUM_PRINTER(MLIL_LSL)
		ENUM_PRINTER(MLIL_LSR)
		ENUM_PRINTER(MLIL_ASR)
		ENUM_PRINTER(MLIL_ROL)
		ENUM_PRINTER(MLIL_RLC)
		ENUM_PRINTER(MLIL_ROR)
		ENUM_PRINTER(MLIL_RRC)
		ENUM_PRINTER(MLIL_MUL)
		ENUM_PRINTER(MLIL_MULU_DP)
		ENUM_PRINTER(MLIL_MULS_DP)
		ENUM_PRINTER(MLIL_DIVU)
		ENUM_PRINTER(MLIL_DIVU_DP)
		ENUM_PRINTER(MLIL_DIVS)
		ENUM_PRINTER(MLIL_DIVS_DP)
		ENUM_PRINTER(MLIL_MODU)
		ENUM_PRINTER(MLIL_MODU_DP)
		ENUM_PRINTER(MLIL_MODS)
		ENUM_PRINTER(MLIL_MODS_DP)
		ENUM_PRINTER(MLIL_NEG)
		ENUM_PRINTER(MLIL_NOT)
		ENUM_PRINTER(MLIL_SX)
		ENUM_PRINTER(MLIL_ZX)
		ENUM_PRINTER(MLIL_LOW_PART)
		ENUM_PRINTER(MLIL_JUMP)
		ENUM_PRINTER(MLIL_JUMP_TO)
		ENUM_PRINTER(MLIL_CALL)
		ENUM_PRINTER(MLIL_CALL_UNTYPED)
		ENUM_PRINTER(MLIL_CALL_OUTPUT)
		ENUM_PRINTER(MLIL_CALL_PARAM)
		ENUM_PRINTER(MLIL_RET)
		ENUM_PRINTER(MLIL_NORET)
		ENUM_PRINTER(MLIL_IF)
		ENUM_PRINTER(MLIL_GOTO)
		ENUM_PRINTER(MLIL_CMP_E)
		ENUM_PRINTER(MLIL_CMP_NE)
		ENUM_PRINTER(MLIL_CMP_SLT)
		ENUM_PRINTER(MLIL_CMP_ULT)
		ENUM_PRINTER(MLIL_CMP_SLE)
		ENUM_PRINTER(MLIL_CMP_ULE)
		ENUM_PRINTER(MLIL_CMP_SGE)
		ENUM_PRINTER(MLIL_CMP_UGE)
		ENUM_PRINTER(MLIL_CMP_SGT)
		ENUM_PRINTER(MLIL_CMP_UGT)
		ENUM_PRINTER(MLIL_TEST_BIT)
		ENUM_PRINTER(MLIL_BOOL_TO_INT)
		ENUM_PRINTER(MLIL_ADD_OVERFLOW)
		ENUM_PRINTER(MLIL_SYSCALL)
		ENUM_PRINTER(MLIL_SYSCALL_UNTYPED)
		ENUM_PRINTER(MLIL_TAILCALL)
		ENUM_PRINTER(MLIL_TAILCALL_UNTYPED)
		ENUM_PRINTER(MLIL_SEPARATE_PARAM_LIST)
		ENUM_PRINTER(MLIL_SHARED_PARAM_SLOT)
		ENUM_PRINTER(MLIL_BP)
		ENUM_PRINTER(MLIL_TRAP)
		ENUM_PRINTER(MLIL_UNDEF)
		ENUM_PRINTER(MLIL_UNIMPL)
		ENUM_PRINTER(MLIL_UNIMPL_MEM)
		ENUM_PRINTER(MLIL_SET_VAR_SSA)
		ENUM_PRINTER(MLIL_SET_VAR_SSA_FIELD)
		ENUM_PRINTER(MLIL_SET_VAR_SPLIT_SSA)
		ENUM_PRINTER(MLIL_SET_VAR_ALIASED)
		ENUM_PRINTER(MLIL_SET_VAR_ALIASED_FIELD)
		ENUM_PRINTER(MLIL_VAR_SSA)
		ENUM_PRINTER(MLIL_VAR_SSA_FIELD)
		ENUM_PRINTER(MLIL_VAR_ALIASED)
		ENUM_PRINTER(MLIL_VAR_ALIASED_FIELD)
		ENUM_PRINTER(MLIL_CALL_SSA)
		ENUM_PRINTER(MLIL_CALL_UNTYPED_SSA)
		ENUM_PRINTER(MLIL_SYSCALL_SSA)
		ENUM_PRINTER(MLIL_SYSCALL_UNTYPED_SSA)
		ENUM_PRINTER(MLIL_TAILCALL_SSA)
		ENUM_PRINTER(MLIL_TAILCALL_UNTYPED_SSA)
		ENUM_PRINTER(MLIL_CALL_PARAM_SSA)
		ENUM_PRINTER(MLIL_CALL_OUTPUT_SSA)
		ENUM_PRINTER(MLIL_LOAD_SSA)
		ENUM_PRINTER(MLIL_LOAD_STRUCT_SSA)
		ENUM_PRINTER(MLIL_STORE_SSA)
		ENUM_PRINTER(MLIL_STORE_STRUCT_SSA)
		ENUM_PRINTER(MLIL_VAR_PHI)
		ENUM_PRINTER(MLIL_MEM_PHI)
	default:
		LogInfo("<invalid operation %" PRId32 ">", operation);
		break;
	}
}


void PrintVariable(MediumLevelILFunction* func, const Variable& var)
{
	string name = func->GetFunction()->GetVariableName(var);
	if (name.size() == 0)
		LogInfo("<no name>");
	else
		LogInfo("%s", name.c_str());
}


void PrintMLILExpr(const MediumLevelILInstruction& instr, size_t indent)
{
	PrintIndent(indent);
	PrintMLILOperation(instr.operation);
	LogInfo("\n");

	indent++;

	for (auto& operand : instr.GetOperands())
	{
		switch (operand.GetType())
		{
		case IntegerMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("int 0x%" PRIx64 "\n", operand.GetInteger());
			break;

		case IndexMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("index %" PRIdPTR "\n", operand.GetIndex());
			break;

		case ExprMediumLevelOperand:
			PrintMLILExpr(operand.GetExpr(), indent);
			break;

		case VariableMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("var ");
			PrintVariable(instr.function, operand.GetVariable());
			LogInfo("\n");
			break;

		case SSAVariableMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("ssa var ");
			PrintVariable(instr.function, operand.GetSSAVariable().var);
			LogInfo("#%" PRIdPTR "\n", operand.GetSSAVariable().version);
			break;

		case IndexListMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("index list ");
			for (auto i : operand.GetIndexList())
				LogInfo("%" PRIdPTR " ", i);
			LogInfo("\n");
			break;

		case VariableListMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("var list ");
			for (auto i : operand.GetVariableList())
			{
				PrintVariable(instr.function, i);
				LogInfo(" ");
			}
			LogInfo("\n");
			break;

		case SSAVariableListMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("ssa var list ");
			for (auto i : operand.GetSSAVariableList())
			{
				PrintVariable(instr.function, i.var);
				LogInfo("#%" PRIdPTR " ", i.version);
			}
			LogInfo("\n");
			break;

		case ExprListMediumLevelOperand:
			PrintIndent(indent);
			LogInfo("expr list\n");
			for (auto& i : operand.GetExprList())
				PrintMLILExpr(i, indent + 1);
			break;

		default:
			PrintIndent(indent);
			LogInfo("<invalid operand>\n");
			break;
		}
	}
}

void AnalyzeMLILFunction(const BinaryNinja::Ref<BinaryNinja::Function>& func) {
	// Get the name of the function and display it
	Ref<Symbol> sym = func->GetSymbol();
	if (sym)
		LogInfo("Function %s:\n", sym->GetFullName().c_str());
	else
		LogInfo("Function at 0x%" PRIx64 ":\n", func->GetStart());

	// Fetch the medium level IL for the function
	Ref<MediumLevelILFunction> il = func->GetMediumLevelIL();
	if (!il)
	{
		LogInfo("    Does not have MLIL\n\n");
		return;
	}

	// Loop through all blocks in the function
	for (auto& block : il->GetBasicBlocks())
	{
		// Loop though each instruction in the block
		for (size_t instrIndex = block->GetStart(); instrIndex < block->GetEnd(); instrIndex++)
		{
			// Fetch IL instruction
			MediumLevelILInstruction instr = (*il)[instrIndex];

			// Display core's intrepretation of the IL instruction
			vector<InstructionTextToken> tokens;
			il->GetInstructionText(func, func->GetArchitecture(), instrIndex, tokens);
			LogInfo("    %" PRIdPTR " @ 0x%" PRIx64 "  ", instrIndex, instr.address);
			for (auto& token : tokens)
				LogInfo("%s", token.text.c_str());
			LogInfo("\n");

			// Generically parse the IL tree and display the parts
			PrintMLILExpr(instr, 2);

			// Example of using visitors to find all constants in the instruction
			instr.VisitExprs([&](const MediumLevelILInstruction& expr) {
				bool status = MediumLevelILFunction::IsConstantType(expr.operation);
				if (status)
					LogInfo("        Found constant 0x%" PRIx64 "\n", expr.GetConstant());
				return !status;
			});

			// Example of using the templated accessors for efficiently parsing load instructions
			instr.VisitExprs([&](const MediumLevelILInstruction& expr) {
				switch (expr.operation)
				{
				case MLIL_LOAD:
					if (expr.GetSourceExpr<MLIL_LOAD>().operation == MLIL_CONST_PTR)
					{
						LogInfo("        Loading from address 0x%" PRIx64 "\n",
						    expr.GetSourceExpr<MLIL_LOAD>().GetConstant<MLIL_CONST_PTR>());
						return false;  // Done parsing this
					}
					else if (expr.GetSourceExpr<MLIL_LOAD>().operation == MLIL_EXTERN_PTR)
					{
						LogInfo("        Loading from address 0x%" PRIx64 "\n",
						    expr.GetSourceExpr<MLIL_LOAD>().GetConstant<MLIL_EXTERN_PTR>());
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