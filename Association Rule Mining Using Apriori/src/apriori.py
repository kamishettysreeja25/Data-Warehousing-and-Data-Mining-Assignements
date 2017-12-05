#--------------importing necessary libraries--------------#
import csv
import argparse
from itertools import chain, combinations
from collections import defaultdict

#------------------declaring global variables--------------#
input_obj = ""
output_obj = ""
min_confidence = 0
min_support = 0
flag = 0
#------------------- Reading a CSV file ---------------------#
def csv_reader(file_obj):
    global input_obj
    global output_obj
    global min_confidence
    global min_support
    global flag
    reader = csv.reader(file_obj)
    for row in reader:
        if row[0] == "input":
          input_obj = row[1]
        elif row[0] == "output":
          output_obj = row[1]
        elif row[0] == "confidence":
          min_confidence = row[1]
        elif row[0] == "support":
          min_support = row[1]
        elif row[0] == "flag":
          flag = row[1]
        
#------------------writing to a csv file----------------------#

def csv_writer(data, path):
    with open(path, "wb") as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        for line in data:
            writer.writerow(line)

#------------------- function to join differnt itemset for frequency---------#
def joinset(itemset, k):
    A = set([i.union(j) for i in itemset for j in itemset if len(i.union(j)) == k])
    return A

#-------------------- function which send the subsets ------------------------#
def subsets(itemset):
    A = chain(*[combinations(itemset, i + 1) for i, a in enumerate(itemset)])
    return A

#-------------------- get the itemsets from the data input---------------------#
def itemset_from_data(data):
    itemset = set()
    transaction_list = list()
    for row in data:
	# all the transaction in a set
        transaction_list.append(frozenset(row))
        for item in row:
            if item:
                # distinct items of all the transactions
                itemset.add(frozenset([item]))
    return itemset, transaction_list

#------------------- check the support of the itemset---------------------------#
def itemset_support(transaction_list, itemset, min_support):
    len_transaction_list = len(transaction_list)
    l = [
        (item, float(sum(1 for row in transaction_list if item.issubset(row)))/len_transaction_list) 
        for item in itemset
    ]
    return dict([(item, support) for item, support in l if support >= min_support])

#-------------------- find the frequency of the itemset-------------------------------#
def freq_itemset(transaction_list, c_itemset, min_support):
    f_itemset = dict()
    k = 1
    while True:
        if k > 1:
            c_itemset = joinset(l_itemset, k)
        l_itemset = itemset_support(transaction_list, c_itemset, min_support)
        if not l_itemset:
            break
        f_itemset.update(l_itemset)
        k += 1
    # get all the subsets of the items of different possible sizes which has fre >= min_support
    return f_itemset    

#---------------the main apriori algorithm-------------------------------------------------------------#
def apriori(data, min_support):
    # Get first itemset and transactions
    itemset, transaction_list = itemset_from_data(data)

    # Get the frequent itemset
    f_itemset = freq_itemset(transaction_list, itemset, min_support)
    return f_itemset
  
#------------------------------------------- Association rules-------------------------------------#
def rules_apriori(f_itemset, min_confidence):
    rules = list()
    for item, support in f_itemset.items():
        if len(item) > 1:
            for A in subsets(item):
                B = item.difference(A)
                if B:
                    A = frozenset(A)
                    AB = A | B
                    confidence = float(f_itemset[AB]) / f_itemset[A]
                    if confidence >= min_confidence:
                        rules.append((A, B))    
    return rules

#---------------------- write freq itemset to the output file--------------------------------#
def write_itemset(output_obj, f_itemset):
    listitems = []
    for item, support in sorted(f_itemset.items()):
        listitems.append(list(item))
    listitems.sort()
    listitems.sort(key=len)
    with open(output_obj, "wb") as csv_file:
	csv_file.write(str(len(listitems)))
	csv_file.write("\n")
        writer = csv.writer(csv_file, delimiter=',')
        for item in listitems:
            writer.writerow(item)
            
#------------------------- write both freq items and rules to the output file--------------------#
def print_report(output_obj, rules, f_itemset):
    listitems = []
    for item, support in sorted(f_itemset.items()):
        listitems.append(list(item))
    listitems.sort()
    listitems.sort(key=len)
    
    with open(output_obj, "wb") as csv_file:
    	csv_file.write(str(len(listitems)))
    	csv_file.write("\n")
        writer = csv.writer(csv_file, delimiter=',')
        for item in listitems:
            writer.writerow(item)
    csv_file.close()
    
    dictitems = defaultdict(list)
    count = 0
    
    for A, B in sorted(rules):
        dictitems[tuple(A)].append(tuple(B))
        count = count + 1

    with open(output_obj, "a") as csv_file:
    	csv_file.write(str(count))
    	csv_file.write("\n")
        for k,s in dictitems.iteritems():
            new_list = [','.join(words) for words in k]
            temp = ""
            for val in new_list:
            	temp = temp + str(val) + ","
            new_values = [','.join(words) for words in s]
            #print new_values
            for v in new_values:
             		val = temp
            		val = val + "=>," + str(v)  + "\n"
            		csv_file.write(val)       
    csv_file.close()

#---------------------------- read the data from the input csv file-----------------------#
def data_from_input(input_filename):
    f = open(input_filename, 'rU')
    for l in f:
        row = map(str.strip, l.split(','))
        yield row

#----------------------------- main function -----------------------------------------------#
def main():
 #   options = parse_options()
#   print options.filename
    csv_path = "config.csv"
    with open(csv_path, "rb") as f_obj:
        csv_reader(f_obj)
    f_obj.close()
    data = data_from_input(input_obj)
    final_support = float(min_support)
    final_confidence = float(min_confidence)
    final_flag = int(flag)
    if final_flag == 1:
    	itemset = apriori(data, final_support)
    	rules = rules_apriori(itemset, final_confidence)
    	print_report(output_obj, rules, itemset)
    elif final_flag == 0:
    	itemset = apriori(data, final_support)
    	write_itemset(output_obj, itemset)


if __name__ == '__main__':
    main()

