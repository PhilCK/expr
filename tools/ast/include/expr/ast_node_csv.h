#ifndef AST_CSV_NODE_INCLUDED_
#define AST_CSV_NODE_INCLUDED_


enum expr_ast_node_csv_id {
        EX_AST_CSV_NULL,

        EX_AST_CSV_DOC,

        /* row and row subtypes */
        EX_AST_CSV_ROW,
        EX_AST_CSV_ROW_HEADER,
        EX_AST_CSV_ROW_CONTENT,

        /* cell and cell subtypes */
        EX_AST_CSV_CELL,
        EX_AST_CSV_CELL_STR,
        EX_AST_CSV_CELL_INT,
        EX_AST_CSV_CELL_INT2,
        EX_AST_CSV_CELL_INT3,
        EX_AST_CSV_CELL_INT4,
        EX_AST_CSV_CELL_FLOAT,
        EX_AST_CSV_CELL_FLOAT2,
        EX_AST_CSV_CELL_FLOAT3,
        EX_AST_CSV_CELL_FLOAT4,
        EX_AST_CSV_CELL_BOOL,
};


static const char *expr_ast_node_csv_names[] = {
        "EX_AST_CSV_NULL"
          ,
        "EX_AST_CSV_DOC",

        "EX_AST_CSV_ROW",
        "EX_AST_CSV_ROW_HEADER",
        "EX_AST_CSV_ROW_CONTENT",

        "EX_AST_CSV_CELL",
        "EX_AST_CSV_CELL_STR",
        "EX_AST_CSV_CELL_INT",
        "EX_AST_CSV_CELL_INT2",
        "EX_AST_CSV_CELL_INT3",
        "EX_AST_CSV_CELL_INT4",
        "EX_AST_CSV_CELL_FLOAT",
        "EX_AST_CSV_CELL_FLOAT2",
        "EX_AST_CSV_CELL_FLOAT3",
        "EX_AST_CSV_CELL_FLOAT4",
        "EX_AST_CSV_CELL_BOOL",
};


/* inc guard */
#endif

