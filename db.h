// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdint.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <unistd.h>

typedef struct
{
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum
{
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum
{
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef enum
{
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_UNRECOGNIZED_STATEMENT,
  PREPARE_STRING_TOO_LONG,
  PREPARE_NEGATIVE_ID
} PrepareResult;

typedef enum
{
  EXECUTE_SUCCESS,
  EXECUTE_DUPLICATE_KEY,
  EXECUTE_TABLE_FULL
} ExecuteResult;

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct
{
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct
{
  StatementType type;
  Row row_to_insert;
} Statement;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)

#define TABLE_MAX_PAGES 100

typedef struct
{
  int file_descriptor;
  uint32_t file_length;
  uint32_t num_pages;
  void *pages[TABLE_MAX_PAGES];
} Pager;

typedef struct
{
  Pager *pager;
  uint32_t root_page_num;
} Table;

typedef struct
{
  Table *table;
  uint32_t page_num;
  uint32_t cell_num;
  bool end_of_table;
} Cursor;

typedef enum
{
  NODE_INTERNAL,
  NODE_LEAF
} NodeType;


uint32_t *leaf_node_num_cells(void *node);

void *leaf_node_cell(void *node, uint32_t cell_num);
uint32_t *leaf_node_key(void *node, uint32_t cell_num);
void *leaf_node_value(void *node, uint32_t cell_num);
NodeType get_node_type(void *node);
void set_node_type(void *node, NodeType type);
void print_constans();
void indent(uint32_t level);
InputBuffer *new_input_buffer();
void print_prompt();
void read_input(InputBuffer *input_buffer);
void close_input_buffer(InputBuffer *input_buffer);
void serialize_row(Row *source, void *destination);
void deserialize_row(void *source, Row *destination);
void print_row(Row *row);
bool is_node_root(void *node);
void set_node_root(void *node, bool is_root);
void initialize_leaf_node(void *node);
void initialize_internal_node(void *node);
void *get_page(Pager *pager, uint32_t page_num);
Cursor *table_start(Table *table);
Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key);
Cursor *table_find(Table *table, uint32_t key);
void *cursor_value(Cursor *cursor);
void cursor_advance(Cursor *cursor);
Pager *pager_open(const char *filename);
Table *db_open(const char *filename);
void pager_flush(Pager *pager, uint32_t page_num);
void db_close(Table *table);
void print_tree(Pager *pager, uint32_t page_num, uint32_t indentation_level);
MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table);
uint32_t get_unused_page_num(Pager *pager);
uint32_t *internal_node_num_keys(void *node);
uint32_t *internal_node_right_child(void *node);
uint32_t *internal_node_cell(void *node, uint32_t cell_num);
uint32_t *internal_node_child(void *node, uint32_t child_num);
uint32_t *internal_node_key(void *node, uint32_t key_num);
uint32_t get_node_max_key(void *node);
void create_new_root(Table *table, uint32_t right_child_page_num);
void leaf_node_split_and_insert(Cursor *cursor, uint32_t key, Row *value);
void leaf_node_insert(Cursor *cursor, uint32_t key, Row *value);
PrepareResult prepare_insert(InputBuffer *input_buff, Statement *statement);
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);
ExecuteResult execute_insert(Statement *statement, Table *table);
ExecuteResult execute_select(Statement *statement, Table *table);
ExecuteResult execute_statement(Statement *statement, Table *table);
