#!/usr/bin/python3

# Copyright (C) 2019, Jaguar Land Rover
# This program is licensed under the terms and conditions of the
# Mozilla Public License, version 2.0.  The full text of the
# Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
#
# Author:Steven Martin (smarti24@jaguarlandrover.com)

num_expressions_to_generate = 16
indent = "    "
function_indent = "                 "
nl = "\n        "
comma = ","
end_bracket = "}"

else_txt = "else "
function_start = "void execute(uint8_t* payload, uint16_t payload_len) override {"
conditional_expression = "if constexpr (sizeof...(Types) == {}) {{"
function_call_start = "_function("
empty_function = "return _function();"
define_arg = "auto arg{} = utils::getArgType{}<Types...>(&payload);"
get_args_expr = "arg{}"
function_call_end = ");"
end = "static_assert (sizeof...(Types) <= {}, \"Currently only up to {} paramater for callbacks in supported, although this can be expanded\");"
arg_delete_expression="\n{}{}{}{}if constexpr (isVarPointer(arg{})) delete[] arg{};"

function_text = nl

function_text += function_start

function_text += nl + indent + conditional_expression.format(0)
function_text += nl + indent + indent + empty_function + nl + indent + end_bracket

for idx in range(num_expressions_to_generate):
    function_text += nl + indent

    function_text += else_txt
    function_text += conditional_expression.format(idx + 1) + nl

    for idx2 in range(idx + 1):
        function_text += indent + indent + define_arg.format(idx2, idx2) + nl

    function_text += indent + indent + function_call_start
    for idx2 in range(idx + 1):
        function_text += get_args_expr.format(idx2)
        if (idx2 == (idx)):
            function_text += function_call_end
        else:
            function_text += comma + " "
    for idx2 in range(idx + 1):
        function_text += arg_delete_expression.format(
            indent, indent, indent, indent, idx2, idx2
        )

    function_text += nl + indent + end_bracket

function_text += nl + indent + end.format(num_expressions_to_generate, num_expressions_to_generate) + nl + end_bracket

print (function_text)

