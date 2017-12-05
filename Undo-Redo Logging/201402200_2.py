import sys

def main():
    input_filename = sys.argv[1]
    with open(input_filename,'r') as f:
        lines = filter(None, (line.rstrip() for line in f))
    i = 0
    databases = {}
    database_list = lines[0].split()
    list_db = []
    dict_updated_values = {}
    while i < len(database_list):
        databases[database_list[i]] = database_list[i+1]
        list_db.append(database_list[i])
        i = i + 2

    #print database_list
    for i in range(len(lines)):
      lines[i] = lines[i].replace("<","")
      lines[i] = lines[i].replace(">","")

    #print lines
    ## now traverse from the last
    commit_trans = {}
    list_complete_t = []
    i = len(lines) - 1

    while i > 0:

      if "end" in lines[i]:
        flag = 1
        i_temp = i - 1
        while flag:
          if "ckpt" in lines[i_temp]:
            lines[i_temp] = lines[i_temp].replace(",","")
            list_complete_trans = lines[i_temp].split(" ")
            #print list_complete_trans
            j = 2
            while j < len(list_complete_trans):
              if list_complete_trans[j] not in list_complete_t:
                list_complete_t.append(list_complete_trans[j])
              #flag_trans_complete[list_complete_trans[k]] = 1
              j = j + 1
            flag = 0
          i_temp = i_temp - 1
        #print list_complete_t
      if "commit" in lines[i]:
        lines[i] = lines[i].replace(",","")
        temp = lines[i].split(" ")
        if temp[0] not in list_complete_t:
          list_complete_t.append(temp[0])
      i = i - 1

    list_recovery_database = []
    dict_recovery_values = {}
    i = len(lines) - 1
    while i > 0:
      if len(lines[i].split(" ")) == 3:
        lines[i] = lines[i].replace(",","")
        temp = lines[i].split(" ")
        if temp[0] not in list_complete_t:
          if temp[1] not in list_recovery_database:
            list_recovery_database.append(temp[1])
            dict_recovery_values[temp[1]] = temp[2]
          dict_recovery_values[temp[1]] = temp[2]
      i = i - 1
    i = len(list_recovery_database) - 1
    while i > -1:
      print list_recovery_database[i] + " " + dict_recovery_values[list_recovery_database[i]],
      i = i - 1

if __name__ == "__main__":
    main()
