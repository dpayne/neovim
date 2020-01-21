#include <assert.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "tree_sitter/api.h"
#include "tree_sitter/language.h"

TSLanguage *tree_sitter_c();

void generic_test_fuzz1(const uint8_t *data, size_t size)
{
  struct TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_c());

  // Build a syntax tree based on source code stored in a string.
  TSTree *tree = ts_parser_parse_string(parser, NULL, (const char *)data,
                                        (uint32_t)size);

  /* const char *source_code */
  /*     = "\ */
  /*    #include <stdio.h>\n\ */
	/* int main()\n\ */
	/* {\n\ */
	    /* int i, j, rows;\n\ */
/* \n\ */
	    /* printf(\"Enter number of rows: \");\n\ */
	    /* scanf(\"%d\",&rows);\n\ */
/* \n\ */
	    /* for(i=1; i<=rows; ++i)\n\ */
	    /* {\n\ */
		/* for(j=1; j<=i; ++j)\n\ */
		/* {\n\ */
		    /* printf(\"* \");\n\ */
		/* }\n\ */
		/* printf(\"\\n\");\n\ */
	    /* }\n\ */
	    /* return 0;\n\ */
	/* }\n\ */
  /*    "; */

  /* printf("Code parsed: %s\n", source_code); */

  /* TSTree *tree = ts_parser_parse_string(parser, NULL, source_code, */
  /*                                     (uint32_t)strlen(source_code)); */

  // Get the root node of the syntax tree.
  TSNode root_node = ts_tree_root_node(tree);

  // Get some child nodes.
  /* TSNode array_node = ts_node_named_child(root_node, 0); */
  /* TSNode number_node = ts_node_named_child(array_node, 0); */

  // Check that the nodes have the expected types.
  /* assert(strcmp(ts_node_type(root_node), "value") == 0); */
  /* assert(strcmp(ts_node_type(array_node), "array") == 0); */
  /* assert(strcmp(ts_node_type(number_node), "number") == 0); */

/*   // Check that the nodes have the expected child counts. */
/*   assert(ts_node_child_count(root_node) == 1); */
/*   assert(ts_node_child_count(array_node) == 5); */
/*   assert(ts_node_named_child_count(array_node) == 2); */
/*   assert(ts_node_child_count(number_node) == 0); */

  // Print the syntax tree as an S-expression.
  char *string = ts_node_string(root_node);
  /* printf("Syntax tree: %s\n", string); */

  // Free all of the heap-allocated memory.
  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
}

int LLVMFuzzerInitialize(int *argc, char ***argv);
int LLVMFuzzerInitialize(int *argc, char ***argv)
{
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
  generic_test_fuzz1(data, size);
  return 0;
}
