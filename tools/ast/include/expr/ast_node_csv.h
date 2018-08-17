#ifndef AST_CSV_NODE_INCLUDED_
#define AST_CSV_NODE_INCLUDED_


enum expr_ast_node_csv_id {
        /* row and row subtypes */
        EX_AST_CSV_ROW,
        EX_AST_CSV_ROW_HEADER,
        EX_AST_CSV_ROW_CONTENT,

        /* cell and cell subtypes */
        EX_AST_CSV_CELL,
        EX_AST_CSV_CELL_UNKNOWN,
        EX_AST_CSV_CELL_STR,
        EX_AST_CSV_CELL_INT,
        EX_AST_CSV_CELL_INT2,
        EX_AST_CSV_CELL_INT3,
        EX_AST_CSV_CELL_INT4,
        EX_AST_CSV_CELL_INTX,
        EX_AST_CSV_CELL_FLOAT,
        EX_AST_CSV_CELL_FLOAT2,
        EX_AST_CSV_CELL_FLOAT3,
        EX_AST_CSV_CELL_FLOAT4,
        EX_AST_CSV_CELL_FLOATX,
        EX_AST_CSV_CELL_BOOL,

        /* delim type */
        EX_AST_CSV_DELIM,
};


/* inc guard */
#endif

