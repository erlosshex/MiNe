import sys
import os
import shutil
import numpy as np

this_debug = 1
this_create = 1

pre_train_file_str = "train_images"
pre_valid_file_str = "valid_images"
pre_train_text_str = "train_labels.txt"
pre_valid_text_str = "valid_labels.txt"
save_file_path = ""
pre_positive_str = "positive_"
pre_negative_str = "neagative_"

random_flag = False

positive_negative_file = []
positive_image_path = ""
negative_image_path = ""
positive_label_path = ""
negative_label_path = ""
positive_num = 0
negative_num = 0

positive_file_name = []
negative_file_name = []
positive_label_name = []
negative_label_name = []

train_positive_file_name = []
train_negative_file_name = []
valid_positive_file_name = []
valid_negative_file_name = []

train_positive_labels_list = []
train_negative_labels_list = []
valid_positive_labels_list = []
valid_negative_labels_list = []

train_positive_index_list = []
train_negative_index_list = []
valid_positive_index_list = []
valid_negative_index_list = []

# parameter's flag
param_help_flag = False
param_random_flag = False
param_create_train_valid_meanwhile_flag = True
param_tpositive_num_flag = False
param_tpositive_num_int = 0
param_tnegative_num_flag = False
param_tnegative_num_int = 0
param_vpositive_num_flag = False
param_vpositive_num_int = 0
param_vnegative_num_flag = False
param_vnegative_num_int = 0
param_according_image_flag = False
param_no_cross_flag = False

# parameter's type
param_help_str = '--help'
param_random_str = '--random'
param_create_train_valid_meanwhile_str = '--traval'
param_tpositive_num_str = '--tposnum'
param_tnegative_num_str = '--tnegnum'
param_vpositive_num_str = '--vposnum'
param_vnegative_num_str = '--vnegnum'
param_according_image_str = '--accimg'
param_no_cross_str = '--ncross'

def help():
    print("-"*50)
    print("create_imageset_train_test.py")
    print("-- help --")
    print("examples : create_imageset_train_test.py [options] positive_imageset_directory negative_imageset_directory [positive_imageset_labels_file] [negative_imageset_labels_file]")
    print("options:")
    print("    --help      :    if true, print help message, if false, nothing done")
    print("    --random    :    select samples randomly flag")
    print("    --accimg    :    if true, select train and valid set according image set, if false, select train and valid set according label set")
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
    global positive_negative_file
    analysis_dict = {}
    for p in params:
        if "=" in p:
            res_split = p.split("=")
            analysis_dict[res_split[0]] = res_split[1]
        else:
            positive_negative_file.append(p)

    #  help
    global param_help_str
    global param_help_flag
    if param_help_str in analysis_dict:
        res = analysis_dict[param_help_str]
        if res == "true" or  res == "True" or  res == "t":
            param_help_flag = True
        else:
            param_help_flag = False
    else:
        param_help_flag = False

    #  random
    global param_random_str
    global param_random_flag
    if param_random_str in analysis_dict:
        res = analysis_dict[param_random_str]
        if res == "true" or  res  == "True" or  rrres == "t":
            param_random_flag = True
        else:
            param_random_flag = False
    else:
        param_random_flag = False

    #  according image
    global param_according_image_flag
    global param_according_image_str
    if param_according_image_str in analysis_dict:
        res = analysis_dict[param_according_image_str]
        if res == "true" or  res  == "True" or  res  == "t":
            param_according_image_flag = True
        else:
            param_according_image_flag = False
    else:
        param_according_image_flag = False

    #  create train and valid set meanwhile
    global param_create_train_valid_meanwhile_flag
    global param_create_train_valid_meanwhile_str
    if param_create_train_valid_meanwhile_str in analysis_dict:
        res = analysis_dict[param_create_train_valid_meanwhile_str]
        if res == "true" or  res == "True" or   res == "t":
            param_create_train_valid_meanwhile_flag = True
        else:
            param_create_train_valid_meanwhile_flag = False
    else:
        param_create_train_valid_meanwhile_flag = False


    #  train positive sample number
    global param_tpositive_num_flag
    global param_tpositive_num_int
    global param_tpositive_num_str
    if param_tpositive_num_str in analysis_dict:
        res = analysis_dict[param_tpositive_num_str]
        res = int(res)
        param_tpositive_num_flag = True
        param_tpositive_num_int = res
    else:
        param_tpositive_num_flag = False

    #  train negative sample number
    global param_tnegative_num_flag
    global param_tnegative_num_int
    global param_tnegative_num_str
    if param_tnegative_num_str in analysis_dict:
        res = analysis_dict[param_tnegative_num_str]
        res = int(res)
        param_tnegative_num_flag = True
        param_tnegative_num_int = res
    else:
        param_tnegative_num_flag = False


    #  valid positive sample number
    global param_vpositive_num_flag
    global param_vpositive_num_int
    global param_vpositive_num_str
    if param_vpositive_num_str in analysis_dict:
        res = analysis_dict[param_vpositive_num_str]
        res = int(res)
        param_vpositive_num_flag = True
        param_vpositive_num_int = res
    else:
        param_vpositive_num_flag = False


    #  valid negative sample number
    global param_vnegative_num_flag
    global param_vnegative_num_int
    global param_vnegative_num_str
    if param_vnegative_num_str in analysis_dict:
        res = analysis_dict[param_vnegative_num_str]
        res = int(res)
        param_vnegative_num_flag = True
        param_vnegative_num_int = res
    else:
        param_vnegative_num_flag = False

    #  path
    global positive_image_path
    global negative_image_path
    global positive_label_path
    global negative_label_path
    if len(positive_negative_file) >= 2:
        positive_image_path = positive_negative_file[0]
        negative_image_path = positive_negative_file[1]

    if len(positive_negative_file) == 4:
        positive_label_path = positive_negative_file[2]
        negative_label_path = positive_negative_file[3]

    #print(analysis_dict)
    #print(positive_negative_file)



def create_train_valid_list():
    global positive_index_arr
    global negative_index_arr
    global train_positive_index_list
    global valid_positive_index_list
    global train_negative_index_list
    global valid_negative_index_list
    if param_no_cross_flag == True:
        if param_tpositive_num_int + param_vpositive_num_int > positive_num:
            print("positive image set is not enough!")
        elif param_tnegative_num_int + param_vnegative_num_int > negative_num:
            print("negative image set is not enough!")
        else:
            print("create positive and negative index list ...")

            if param_random_flag == True:
                positive_index_arr = generate_random_permutation(positive_num)
                negative_index_arr = generate_random_permutation(negative_num)
            else:
                positive_index_arr = list(range(positive_num))
                negative_index_arr = list(range(negative_num))

            print("positive_index_arr len : ", len(positive_index_arr))
            print("negative_index_arr len : ", len(negative_index_arr))

            if param_create_train_valid_meanwhile_flag == False:
                valid_positive_index_list = []
                valid_negative_index_list = []

            if param_tpositive_num_flag == True:
                train_positive_index_list = positive_index_arr[:param_tpositive_num_int]
            if param_vpositive_num_flag == True:
                valid_positive_index_list = positive_index_arr[param_tpositive_num_int:param_tpositive_num_int+param_vpositive_num_int]
            if param_tnegative_num_flag == True:
                train_negative_index_list = negative_index_arr[:param_tnegative_num_int]
            if param_vnegative_num_flag == True:
                valid_negative_index_list = negative_index_arr[param_tnegative_num_int:param_tnegative_num_int+param_vnegative_num_int]

            if param_tpositive_num_flag == False:
                train_positive_index_list = positive_index_arr[param_tpositive_num_int+param_vpositive_num_int:]
            if param_vpositive_num_flag == False:
                valid_position_index_list = positive_index_arr[param_tpositive_num_int+param_vpositive_num_int:]
            if param_tnegative_num_flag == False:
                train_negative_index_list = negative_index_arr[param_tnegative_num_int+param_vnegative_num_int:]
            if param_vnegative_num_flag == False:
                valid_negative_index_list = negative_index_arr[param_tnegative_num_int+param_vnegative_num_int:]

    else:
        if max(param_tpositive_num_int, param_vpositive_num_int) > positive_num:
            print("positive image set is not enough!")
        elif max(param_tnegative_num_int, param_vnegative_num_int) > negative_num:
            print("negative image set is not enough!")
        else:
            print("create positive and negative index list ...")

            if param_random_flag == True:
                positive_index_arr = generate_random_permutation(positive_num)
                negative_index_arr = generate_random_permutation(negative_num)
            else:
                positive_index_arr = list(range(positive_num))
                negative_index_arr = list(range(negative_num))

            print("positive_index_arr len : ", len(positive_index_arr))
            print("negative_index_arr len : ", len(negative_index_arr))

            if param_create_train_valid_meanwhile_flag == False:
                valid_positive_index_list = []
                valid_negative_index_list = []

            if param_tpositive_num_flag == True:
                train_positive_index_list = positive_index_arr[:param_tpositive_num_int]
            if param_vpositive_num_flag == True:
                valid_positive_index_list = positive_index_arr[len(positive_index_arr)-param_vpositive_num_int:]
            if param_tnegative_num_flag == True:
                train_negative_index_list = negative_index_arr[:param_tnegative_num_int]
            if param_vnegative_num_flag == True:
                valid_negative_index_list = negative_index_arr[len(negative_index_arr)-param_vnegative_num_int:]

            if param_tpositive_num_flag == False:
                train_positive_index_list = positive_index_arr[:len(positive_index_arr)-param_vpositive_num_int]
            if param_vpositive_num_flag == False:
                valid_positive_index_list = positive_index_arr[param_tpositive_num_int:]
            if param_tnegative_num_flag == False:
                train_negative_index_list = negative_index_arr[:len(negative_index_arr)-param_vnegative_num_int]
            if param_vnegative_num_flag == False:
                valid_negative_index_list = negative_index_arr[param_tnegative_num_int:]



def generate_random_permutation(n):
    arr = np.arange(n)
    np.random.shuffle(arr)
    return arr

def select_from_list_with_index(file_list,index_list):
    return extract_list_from_list_with_list(file_list,index_list)

def write_image_name_to_file(file_name,name_list):
    with open(save_file_path+'/'+file_name,'a') as f:
        for i in name_list:
            f.write(i+'\n')

def copy_image_from_path_to_path(src_path,dst_path,src_copy_file_list,dst_copy_file_list):
    #copy_file_list = file_list[index_list]
    copy_file_list = zip(src_copy_file_list,dst_copy_file_list)
    for this_file in copy_file_list:
        old_file_path = src_path + '/' + this_file[0]
        new_file_path = dst_path + '/' + this_file[1]
        shutil.copyfile(old_file_path,new_file_path)

def find_last(ss,s):
    last_position = -1
    while True:
        pos = ss.find(s,last_position+1)
        if pos == -1:
            return last_position
        last_position = pos

def split_path_and_name(p):
    pos = find_last(p,'/')
    path_str = p[:pos]
    name_str = p[pos+1:]
    return (path_str,name_str)

def add_pre_and_suf_str(s,pre=None,suf=None):
    res = ""
    if '/' in s:
        pos1 = find_last(s,'/')
        pos2 = find_last(s,'.')
        s1 = s[:pos1+1]
        s2 = s[pos1+1:pos2]
        s3 = s[pos2:]
        res = s2[:]
        if pre != None:
            res = pre+res
        if suf != None:
            res = res+suf
        res = s1+res+s3
    else:
        res = s[:]
        if pre != None:
            res = pre+res
        if suf != None:
            res = res+suf

    return res

def add_pre_and_suf_str_list(s_list,pre=None,suf=None):
    res = []
    for i in s_list:
        tmp = add_pre_and_suf_str(i,pre,suf)
        res.append(tmp)
    return res

def create_train_valid_no_labels():
    positive_path_str,positive_name_str = split_path_and_name(positive_image_path)
    negative_path_str,negative_name_str = split_path_and_name(negative_image_path)
    global train_positive_file_name
    global valid_positive_file_name
    global train_negative_file_name
    global valid_negative_file_name

    if param_tpositive_num_flag == True:
        train_positive_file_name = select_from_list_with_index(positive_file_name,train_positive_index_list)
        train_positive_file_name_pre = add_pre_and_suf_str_list(train_positive_file_name,pre_positive_str)
        copy_image_from_path_to_path(save_file_path+'/'+positive_image_path,save_file_path+'/'+pre_train_file_str,train_positive_file_name,train_positive_file_name_pre)

    if param_vpositive_num_flag == True or param_create_train_valid_meanwhile_flag == True:
        valid_positive_file_name = select_from_list_with_index(positive_file_name,valid_positive_index_list)
        valid_positive_file_name_pre = add_pre_and_suf_str_list(valid_positive_file_name,pre_positive_str)
        copy_image_from_path_to_path(save_file_path+'/'+positive_image_path,save_file_path+'/'+pre_valid_file_str,valid_positive_file_name,valid_positive_file_name_pre)

    if param_tnegative_num_flag == True:
        train_negative_file_name = select_from_list_with_index(negative_file_name,train_negative_index_list)
        train_negative_file_name_pre = add_pre_and_suf_str_list(train_negative_file_name,pre_negative_str)
        copy_image_from_path_to_path(save_file_path+'/'+negative_image_path,save_file_path+'/'+pre_train_file_str,train_negative_file_name,train_negative_file_name_pre)

    if param_vnegative_num_flag == True or  param_create_train_valid_meanwhile_flag == True:
        valid_negative_file_name = select_from_list_with_index(negative_file_name,valid_negative_index_list)
        valid_negative_file_name_pre = add_pre_and_suf_str_list(valid_negative_file_name,pre_negative_str)
        copy_image_from_path_to_path(save_file_path+'/'+negative_image_path,save_file_path+'/'+pre_valid_file_str,valid_negative_file_name,valid_negative_file_name_pre)


def extract_list_from_list_with_list(list,index):
    res = []
    for i in index:
        res.append(list[i])

    return res

def extract_image_name_from_str(s):
    image_name,_ = s.split(' ')
    return image_name

def extract_label_from_image_name_list(label_list,name_list):
    res_list = []
    for name in name_list:
        for label in label_list:
            if name in label:
                res_list.append(label)
                continue
    return res_list

def create_train_valid_with_labels():
    global train_positive_file_name
    global valid_positive_file_name
    global train_negative_file_name
    global valid_negative_file_name
    global train_positive_labels_list
    global valid_positive_labels_list
    global train_negative_labels_list
    global valid_negative_labels_list
    if param_according_image_flag == True:
        print("according image true")

        if param_tpositive_num_flag == True:
            train_positive_file_name = extract_list_from_list_with_list(positive_file_name,train_positive_index_list)
            train_positive_file_name_pre = add_pre_and_suf_str_list(train_positive_file_name,pre_positive_str)
            copy_image_from_path_to_path(save_file_path+'/'+positive_image_path,save_file_path+'/'+pre_train_file_str,train_positive_file_name,train_positive_file_name_pre)
            train_positive_labels_list = extract_label_from_image_name_list(positive_label_name,train_positive_file_name)
            train_positive_labels_list = add_pre_and_suf_str_list(train_positive_labels_list,pre_positive_str)
            write_image_name_to_file(pre_train_text_str,train_positive_labels_list)

        if param_vpositive_num_flag == True or  param_create_train_valid_meanwhile_flag == True:
            valid_positive_file_name = extract_list_from_list_with_list(positive_file_name,valid_positive_index_list)
            valid_positive_file_name_pre = add_pre_and_suf_str_list(valid_positive_file_name,pre_positive_str)
            copy_image_from_path_to_path(save_file_path+'/'+positive_image_path,save_file_path+'/'+pre_valid_file_str,valid_positive_file_name,valid_positive_file_name_pre)
            valid_positive_labels_list = extract_label_from_image_name_list(positive_label_name,valid_positive_file_name)
            valid_positive_labels_list = add_pre_and_suf_str_list(valid_positive_labels_list,pre_positive_str)
            write_image_name_to_file(pre_valid_text_str,valid_positive_labels_list)

        if param_tnegative_num_flag == True:
            train_negative_file_name = extract_list_from_list_with_list(negative_file_name,train_negative_index_list)
            train_negative_file_name_pre = add_pre_and_suf_str_list(train_negative_file_name,pre_negative_str)
            copy_image_from_path_to_path(save_file_path+'/'+negative_image_path,save_file_path+'/'+pre_train_file_str,train_negative_file_name,train_negative_file_name_pre)
            train_negative_labels_list = extract_label_from_image_name_list(negative_label_name,train_negative_file_name)
            train_negative_labels_list = add_pre_and_suf_str_list(train_negative_labels_list,pre_negative_str)
            write_image_name_to_file(pre_train_text_str,train_negative_labels_list)

        if param_vnegative_num_flag == True or  param_create_train_valid_meanwhile_flag == True:
            valid_negative_file_name = extract_list_from_list_with_list(negative_file_name,valid_negative_index_list)
            valid_negative_file_name_pre = add_pre_and_suf_str_list(valid_negative_file_name,pre_negative_str)
            copy_image_from_path_to_path(save_file_path+'/'+negative_image_path,save_file_path+'/'+pre_valid_file_str,valid_negative_file_name,valid_negative_file_name_pre)
            valid_negative_labels_list = extract_label_from_image_name_list(negative_label_name,valid_negative_file_name)
            valid_negative_labels_list = add_pre_and_suf_str_list(valid_negative_labels_list,pre_negative_str)
            write_image_name_to_file(pre_valid_text_str,valid_negative_labels_list)

    else:
        print("according label true")
        if param_tpositive_num_flag == True:
            train_positive_labels_list = extract_list_from_list_with_list(positive_label_name,train_positive_index_list)
            train_positive_labels_list = add_pre_and_suf_str_list(train_positive_labels_list,pre_positive_str)
            write_image_name_to_file(pre_train_text_str,train_positive_labels_list)
            train_positive_file_name = list(map(extract_image_name_from_str,train_positive_labels_list))
            train_positive_file_name_pre = add_pre_and_suf_str_list(train_positive_file_name,pre_positive_str)
            copy_image_from_path_to_path(save_file_path+'/'+positive_image_path,save_file_path+'/'+pre_train_file_str,train_positive_file_name,train_positive_file_name_pre)

        if param_vpositive_num_flag == True or  param_create_train_valid_meanwhile_flag == True:
            valid_positive_labels_list = extract_list_from_list_with_list(positive_label_name,valid_positive_index_list)
            valid_positive_labels_list = add_pre_and_suf_str_list(valid_positive_labels_list,pre_positive_str)
            write_image_name_to_file(pre_valid_text_str,valid_positive_labels_list)
            valid_positive_file_name = list(map(extract_image_name_from_str,valid_positive_labels_list))
            valid_positive_file_name_pre = add_pre_and_suf_str_list(valid_positive_file_name,pre_positive_str)
            copy_image_from_path_to_path(save_file_path+'/'+positive_image_path,save_file_path+'/'+pre_valid_file_str,valid_positive_file_name,valid_positive_file_name_pre)

        if param_tnegative_num_flag == True:
            train_negative_labels_list = extract_list_from_list_with_list(negative_label_name,train_negative_index_list)
            train_negative_labels_list = add_pre_and_suf_str_list(train_negative_labels_list,pre_negative_str)
            write_image_name_to_file(pre_train_text_str,train_negative_labels_list)
            train_negative_file_name = list(map(extract_image_name_from_str,train_negative_labels_list))
            train_negative_file_name_pre = add_pre_and_suf_str_list(train_negative_file_name,pre_negative_str)
            copy_image_from_path_to_path(save_file_path+'/'+negative_image_path,save_file_path+'/'+pre_train_file_str,train_negative_file_name,train_negative_file_name_pre)

        if param_vnegative_num_flag == True or  param_create_train_valid_meanwhile_flag == True:
            valid_negative_labels_list = extract_list_from_list_with_list(negative_label_name,valid_negative_index_list)
            valid_negative_labels_list = add_pre_and_suf_str_list(valid_negative_labels_list,pre_negative_str)
            write_image_name_to_file(pre_valid_text_str,valid_negative_labels_list)
            valid_negative_file_name = list(map(extract_image_name_from_str,valid_negative_labels_list))
            valid_negative_file_name_pre = add_pre_and_suf_str_list(valid_negative_file_name,pre_negative_str)
            copy_image_from_path_to_path(save_file_path+'/'+negative_image_path,save_file_path+'/'+pre_valid_file_str,valid_negative_file_name,valid_negative_file_name_pre)


def get_positive_file_name():
    global positive_file_name
    positive_file_name = os.listdir(positive_image_path)
    return positive_file_name

def get_positive_label_name():
    global positive_label_name
    f = open(positive_label_path)
    res = f.readlines()
    f.close()
    positive_label_name = list(map(lambda x: x[:len(x)-1] if '\n' in x else x,res))
    return positive_label_name

def get_negative_file_name():
    global negative_file_name
    negative_file_name = os.listdir(negative_image_path)
    return negative_file_name

def get_negative_label_name():
    global negative_label_name
    f = open(negative_label_path)
    res = f.readlines()
    f.close()
    negative_label_name = list(map(lambda x: x[:len(x)-1] if '\n' in x else x, res))
    return negative_label_name

def create_train_valid():
    global file_num
    file_num = len(positive_negative_file)
    if file_num != 2 and  file_num != 4:
        print("positive and negative files input error!")
    else:
        get_positive_file_name()
        get_positive_label_name()
        get_negative_file_name()
        get_negative_label_name()
        global positive_num
        global negative_num
        positive_num = len(positive_file_name)
        negative_num = len(negative_file_name)
        create_train_valid_list()
        if file_num == 2:
            create_train_valid_no_labels()
        else:
            create_train_valid_with_labels()


def get_current_script_cwd():
    global save_file_path
    save_file_path = os.getcwd()


def make_directory(directory_str):
    os.mkdir(directory_str)



def del_dir_tree(file_path):
    if os.path.isfile(file_path):
        try:
            os.remove(file_path)
        except Exception:
            print("error file")
    elif os.path.isdir(file_path):
        for item in os.listdir(file_path):
            item_path = os.path.join(file_path,item)
            del_dir_tree(item_path)
        try:
            os.rmdir(file_path)
        except Exception:
            print("error directory")


if __name__ == "__main__":
    get_current_script_cwd()
    print("current save path : ",save_file_path)

    if os.path.exists(save_file_path+'/'+pre_train_file_str):
        del_dir_tree(save_file_path+'/'+pre_train_file_str)
    if os.path.exists(save_file_path+'/'+pre_valid_file_str):
        del_dir_tree(save_file_path+'/'+pre_valid_file_str)
    make_directory(save_file_path+'/'+pre_train_file_str)
    print("create train image file done!")
    make_directory(save_file_path+'/'+pre_valid_file_str)
    print("create valid image file done!")

    if os.path.exists(save_file_path+'/'+pre_train_text_str):
        os.remove(save_file_path+'/'+pre_train_text_str)
    if os.path.exists(save_file_path+'/'+pre_valid_text_str):
        os.remove(save_file_path+'/'+pre_valid_text_str)

    if this_debug:
        print_script_message()
    analysis_parameters(sys.argv[1:])
    if param_help_flag:
        help()
    if this_create:
        create_train_valid()
    print(len(positive_file_name))
    print(len(positive_label_name))
    print(len(negative_file_name))
    print(len(negative_label_name))
