#!/usr/bin/python3

# Copyright (C) 2019, Jaguar Land Rover
# This program is licensed under the terms and conditions of the
# Mozilla Public License, version 2.0.  The full text of the
# Mozilla Public License is at https://www.mozilla.org/MPL/2.0/
#
# Author:Steven Martin (smarti24@jaguarlandrover.com)

base_indentation = "        "
template_start = "template <"
template_param_format = "typename ARG{}_T,\n"
param_indentation = "          "
template_end = "          typename ... OTHERS>\n{}auto getArgType{}(uint8_t** ptr) {{\n{}    if constexpr (std::is_array<ARG{}_T>::value)\n{}        return parseArgArray<ARG{}_T>(ptr);\n{}    else\n{}        return parseArg<ARG{}_T>(ptr);\n{}}}"

num_params = 16

for param_id in range(num_params):
    template_pattern = base_indentation + template_start
    template_pattern += template_param_format.format("0")
    if (param_id > 0):
        for arg_id in range(1, param_id + 1):
            template_pattern += base_indentation + param_indentation
            template_pattern += template_param_format.format(arg_id)
    template_pattern += base_indentation + template_end.format(base_indentation, param_id, base_indentation, param_id, base_indentation, param_id, base_indentation, base_indentation, param_id, base_indentation)

    print ()
    print(template_pattern)