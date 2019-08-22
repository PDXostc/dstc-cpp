num_expressions_to_generate = 16
indent = "    "
function_indent = "                 "
nl = "\n        "
comma = ","
end_bracket = "}"

else_txt = "else "
function_start = "void execute(uint8_t* payload, uint16_t payload_len) override {"
conditional_expression = "if constexpr (sizeof...(Types) == {}) {{"
return_start = "return _function("
empty_function = "return _function();"
get_args_expr = "utils::getArgType{}<Types...>(payload)"
function_call_end = ");"
end = "static_assert (sizeof...(Types) <= {}, \"Currently only up to {} paramater for callbacks in supported, although this can be expanded\");"

function_text = nl

function_text += function_start

function_text += nl + indent + conditional_expression.format(0)
function_text += nl + indent + indent + empty_function + nl + indent + end_bracket

for idx in range(num_expressions_to_generate):
    function_text += nl + indent

    function_text += else_txt
    function_text += conditional_expression.format(idx + 1) + nl
    function_text += indent + indent + return_start
    for idx2 in range(idx + 1):
        function_text += get_args_expr.format(idx2)
        if (idx2 == (idx)):
            function_text += function_call_end + nl + indent + end_bracket
        else:
            function_text += comma + nl + function_indent + indent + indent

function_text += nl + nl + indent + end.format(num_expressions_to_generate, num_expressions_to_generate) + nl + end_bracket

print (function_text)

