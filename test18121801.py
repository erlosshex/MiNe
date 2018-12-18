import sys

this_debug = 1

random_flag = False

# parameter's flag
param_random_flag = False
param_create_train_valid_meanwhile_flag = False
param_tpositive_num_flag = False
param_tpositive_num_int = 0
param_tnegative_num_flag = False
param_tnegative_num_int = 0
param_vpositive_num_flag = False
param_vpositive_num_int = 0
param_vnegative_num_flag = False
param_vnegative_num_int = 0

# parameter's type
param_random_str = '--random'
param_create_train_valid_meanwhile_str = '--traval'
param_tpositive_num_str = '--tposnum'
param_tnegative_num_str = '--tnegnum'
param_vpositive_num_str = '--vposnum'
param_vnegative_num_str = '--vnegnum'

def help():
    print("-"*50)
    print("create_imageset_train_test.py")
    print("-- help --")
    print("examples : create_imageset_train_test.py [options] positive_imageset_directory negative_imageset_directory [positive_imageset_labels_file] [negative_imageset_labels_file]")
    print("options:")
    print("    --random    :    select samples randomly flag")
    print("    --traval    :    if true, create train set and valid set meanwhile, if false, create neither train set nor valid set")
    print("    --tposnum   :    number of positive samples in train set")
    print("    --tnegnum   :    number of negative samples in train set")
    print("    --vposnum   :    number of positive samples in valid set")
    print("    --vnegnum   :    number of negative samples in valid set")
    print("-"*50)

def print_script_message():
    print("script name : ", sys.argv[0])
    for i in range(1, len(sys.argv)):
        print("parameter ", i, " : ", sys.argv[i])

def analysis_parameters(params):
    analysis_dict = {}
    for p in params:
        res_split = p.split('=')
        analysis_dict[res_split[0]] = res_split[1]

    #  random
    if param_random_str in analysis_dict:
        res = analysis_dict[param_random_str]
        if res == "true" or  res  == "True" or  rrres == "t":
            param_random_flag = True
        else:
            param_random_flag = False
    else:
        param_random_flag = False

    #  create train and valid set meanwhile
    if param_create_train_valid_meanwhile_str in analysis_dict:
        res = analysis_dict[param_create_train_valid_meanwhile_str]
        if res == "true" or  res == "True" or   res == "t":
            param_create_train_valid_meanwhile_flag = True
        else:
            param_create_train_valid_meanwhile_flag = False
    else:
        param_create_train_valid_meanwhile_flag = False


    #  train positive sample number
    if param_tpositive_num_str in analysis_dict:
        res = analysis_dict[param_tpositive_num_str]
        res = int(res)
        param_tpositive_num_flag = True
        param_tpositive_num_int = res
    else:
        param_tpositive_num_flag = False

    #  train negative sample number
    if param_tnegative_num_str in analysis_dict:
        res = analysis_dict[param_tnegative_num_str]
        res = int(res)
        param_tnegative_num_flag = True
        param_tnegative_num_int = res
    else:
        param_tnegative_num_flag = False


    #  valid positive sample number
    if param_vpositive_num_str in analysis_dict:
        res = analysis_dict[param_vpositive_num_str]
        res = int(res)
        param_vpositive_num_flag = True
        param_vpositive_num_int = res
    else:
        param_vpositive_num_flag = False


    #  valid negative sample number
    if param_vnegative_num_str in analysis_dict:
        res = analysis_dict[param_vnegative_num_str]
        res = int(res)
        param_vnegative_num_flag = True
        param_vnegative_num_int = res
    else:
        param_vnegative_num_flag = False


if __name__ == "__main__":
    help()
    if this_debug:
        print_script_message()
