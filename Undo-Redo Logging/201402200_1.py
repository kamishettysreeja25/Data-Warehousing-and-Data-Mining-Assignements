#----------------- Part 1: Undo Logging ----------------------
#Write Undo logs into a file for a given set of transactions.
from sys import argv
import sys, getopt
import re

#global database dictionary to update the contents
databases = {}
trans_start = {}
transact_current_index = {}
list_db = []
list_trans = []
list_var = []

def transact(lines, round_robin_no, transactions):

    j = 0
    trans_var = {}
    sumt = 0
    for i in range(len(list_trans)):
        sumt = sumt + int(transactions[list_trans[i]])
    while sumt:
        j = j%int(round_robin_no)
        i = 0

        if transact_current_index[list_trans[j]] == trans_start[list_trans[j]]:
            print "<" + list_trans[j] + ", " + "start>"
            print_database(list_db)
        i = 1
        while i < 4:
            #print trans_var

            if transact_current_index[list_trans[j]] + i < trans_start[list_trans[j]] + int(transactions[list_trans[j]]):
                #print lines[transact_current_index[list_trans[j]] + i]
                if 'read' in lines[transact_current_index[list_trans[j]] + i]:
                    temp = lines[transact_current_index[list_trans[j]] + i].replace("read(","")
                    temp = temp.replace(")","")
                    temp = temp.split(",")
                    trans_var[temp[1]] = databases[temp[0]]

                elif 'write' in lines[transact_current_index[list_trans[j]] + i]:
                    temp = lines[transact_current_index[list_trans[j]] + i].replace("write(","")
                    temp = temp.replace(")","")
                    temp = temp.split(",")
                    print "<" + list_trans[j] + ", " + temp[0] + ", " +  str(databases[temp[0]]) + ">"
                    databases[temp[0]] = trans_var[temp[1]]
                    print_database(list_db)

                elif ":=" in lines[transact_current_index[list_trans[j]] + i]:
                    temp = lines[transact_current_index[list_trans[j]] + i].split(":=")
                    left_op = temp[0].replace(" ","")
                    right_arg = temp[1].replace(" ","")
                    #print right_arg
                    if '+' in right_arg:
                        right_arg = right_arg.split("+")
                        #print right_arg[1]

                        if right_arg[1].isdigit():
                            trans_var[left_op] = int(trans_var[right_arg[0]]) + int(right_arg[1])
                        else:
                            trans_var[left_op] = int(trans_var[right_arg[0]]) + int(trans_var[right_arg[1]])

                    elif '-' in right_arg:
                        right_arg = right_arg.split("-")
                        if right_arg[1].isdigit():
                            trans_var[left_op] = int(trans_var[right_arg[0]]) - int(right_arg[1])
                        else:
                            trans_var[left_op] = int(trans_var[right_arg[0]]) - int(trans_var[right_arg[1]])

                    elif '*' in right_arg:
                        right_arg = right_arg.split("*")
                        if right_arg[1].isdigit():
                            trans_var[left_op] = int(trans_var[right_arg[0]]) * int(right_arg[1])
                        else:
                            trans_var[left_op] = int(trans_var[right_arg[0]]) * int(trans_var[right_arg[1]])

                    elif '/' in right_arg:
                        right_arg = right_arg.split("/")
                        if right_arg[1].isdigit():
                            trans_var[left_op] = int(trans_var[right_arg[0]]) / int(right_arg[1])
                        else:
                            trans_var[left_op] = int(trans_var[right_arg[0]]) / int(trans_var[right_arg[1]])


                    #print temp

                elif "output" in lines[transact_current_index[list_trans[j]] + i]:
                    print "<" + list_trans[j] + ", commit>"
                    print_database(list_db)

            i = i + 1
        transact_current_index[list_trans[j]] = transact_current_index[list_trans[j]] + i - 1
        j = j + 1
        sumt = sumt - 1

def print_database(list_database):
    for i in range(len(list_database)):
        print list_database[i] + " " + str(databases[list_database[i]]) ,
    print ""

def main():
    input_filename = sys.argv[1]
    round_robin_no = sys.argv[2]
    with open(input_filename,'r') as f:
        lines = filter(None, (line.rstrip() for line in f))

    i = 0
    database_list = lines[0].split()
    while i < len(database_list):
        databases[database_list[i]] = database_list[i+1]
        list_db.append(database_list[i])
        i = i + 2
    i = 1
    # sorted dictionary came out
    #---- Now store the transactions in separate lists for each one
    transactions = {}
    while i < len(lines):
        transaction_list = lines[i].split()
        list_trans.append(transaction_list[0])
        transactions[transaction_list[0]] = transaction_list[1]
        transaction_no = transaction_list[1]
        trans_start[transaction_list[0]] = i
        transact_current_index[transaction_list[0]] = i
        i = i + int(transaction_no) + 1
    transact(lines, round_robin_no, transactions)

if __name__ == "__main__":
    main()
