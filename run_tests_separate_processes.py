#!/usr/bin/python3

import subprocess
import os

p = subprocess.Popen(["./run_tests", "--gtest_list_tests"], stdout=subprocess.PIPE)
result = p.communicate()[0].decode()

test_list = []

prefix = ""
for line in result.split("\n"):
    if "." in line:
        prefix = line.strip()
    else:
        test_part = line.strip()
        if test_part != "":
            full_name = "{}{}".format(prefix, test_part)
            test_list.append(str(full_name))

num_failures = 0
for test in test_list:
    print ("Running {}".format(test))
    result = os.system("./run_tests --gtest_filter={}".format(test))
    if (result != 0):
        num_failures += 1
if (num_failures > 0):
    print ("TESTS FAILED!! : Num failures: {} / {}".format(num_failures, len(test_list)))
else:
    print ("All tests PASS.  Number tests ran: {}".format(len(test_list)))
