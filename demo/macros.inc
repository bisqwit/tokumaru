.macro branch_check opc, dest
    opc dest
    .assert >* = >(dest), warning, "branch_check: failed, crosses page"
.endmacro

.macro Jcc dest
	branch_check bcc, dest
.endmacro
.macro Jcs dest
	branch_check bcs, dest
.endmacro
.macro Jeq dest
	branch_check beq, dest
.endmacro
.macro Jne dest
	branch_check bne, dest
.endmacro
.macro Jmi dest
	branch_check bmi, dest
.endmacro
.macro Jpl dest
	branch_check bpl, dest
.endmacro
.macro Jvc dest
	branch_check bvc, dest
.endmacro
.macro Jvs dest
	branch_check bvs, dest
.endmacro

.macro TableWrapCheck table, last_index, message
	.assert >(table) = >(table+(last_index)), warning, message
.endmacro
