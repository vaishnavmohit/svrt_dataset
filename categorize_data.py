"""
Created on Mon Oct 21 10:18:24 2019

@author: mvaishn1
"""

#!/usr/bin/python

from random import sample
from tqdm import tqdm
import shutil
import os

def main():
    
    # directory where all the folders are present
    dir_path = '/users/mvaishn1/data/mvaishn1/svrt_dataset'
    folderlist= [file for file in os.listdir(dir_path) if file.startswith('results_problem_')]
    print('folder list is: %s'%(folderlist))
    num = 200
    for folder in tqdm(folderlist):
        folder_name = '/val-200/'
        from_folder = 'val'
        folder_path = os.path.join(dir_path,folder)
        if not os.path.exists(folder_path+folder_name+'cat_1'):
            os.makedirs(folder_path+folder_name+'cat_1')
        f_tr_1 = folder_path+folder_name+'cat_1/'
        
        '''
        if not os.path.exists(folder_path+'/val/cat_1'):
            os.makedirs(folder_path+'/val/cat_1')
        f_v_1 = folder_path+'/val/cat_1/'
        
        if not os.path.exists(folder_path+'/test/cat_1'):
            os.makedirs(folder_path+'/test/cat_1')
        f_te_1 = folder_path+'/test/cat_1/'
        '''
        
        if not os.path.exists(folder_path+folder_name+'cat_0'):
            os.makedirs(folder_path+folder_name+'cat_0')
        f_tr_0 = folder_path+folder_name+'cat_0/'
        
        '''
        if not os.path.exists(folder_path+'/val/cat_0'):
            os.makedirs(folder_path+'/val/cat_0')
        f_v_0 = folder_path+'/val/cat_0/'
        
        if not os.path.exists(folder_path+'/test/cat_0'):
            os.makedirs(folder_path+'/test/cat_0')
        f_te_0 = folder_path+'/test/cat_0/'
        '''

        print('folder running is: ',folder)
        path_f = os.path.join(dir_path,folder, from_folder+'/cat_1')
        filelist= [file for file in os.listdir(path_f) if file.startswith('sample_1_')]
        filelist_1= [file for file in os.listdir(f_tr_1) if file.startswith('sample_1_')]
        # moving 20000 cat 1items to test folder
        k = num - len(filelist_1)
        if k < 0:
            new_items1 = sample(filelist_1, k = len(filelist_1) - num)
            for file in new_items1:
                os.remove(os.path.join(f_tr_1,file))
        else:
            new_items1 = sample(filelist, k = num - len(filelist_1))
            for file in new_items1:
                shutil.copy(os.path.join(path_f,file),os.path.join(f_tr_1,file))
        print('I completed %s command' %(f_tr_1))
        
        '''
        filelist= [file for file in os.listdir(path_f) if file.startswith('sample_1_')]
        # moving 20000 cat 1 items to validation folder
        new_items2 = sample(filelist, k = num)
        for file2 in new_items2:
            shutil.move(os.path.join(path_f,file2),os.path.join(f_v_1,file2))
        print('I completed %s command' %(f_v_1))
            
        # moving rest of the cat 1 items to train folder
        filelist= [file for file in os.listdir(path_f) if file.startswith('sample_1_')]
        for img in (filelist):
            shutil.move(os.path.join(path_f,img),os.path.join(f_tr_1,img))
        print('I completed %s command' %(f_tr_1))
        
        # for category 2 images:
    
        filelist2= [file for file in os.listdir(path_f) if file.startswith('sample_0_')]
    
        # moving 20000 cat 1items to test folder
        new_items3 = sample(filelist2, k = num)
        for file3 in new_items3:
            shutil.move(os.path.join(path_f,file3),os.path.join(f_te_0,file3))
        print('I completed %s command' %(f_te_0))
        
        filelist4= [file for file in os.listdir(path_f) if file.startswith('sample_0_')]
        # moving 20000 cat 1 items to validation folder
        new_items4 = sample(filelist4, k = num)
        for file4 in new_items4:
            shutil.move(os.path.join(path_f,file4),os.path.join(f_v_0,file4))
        print('I completed %s command' %(f_v_0))
        '''
        # moving rest of the cat 1 items to train folder
        path_f = os.path.join(dir_path,folder, from_folder+'/cat_0')
        filelist5= [file for file in os.listdir(path_f) if file.startswith('sample_0_')]
        filelist5_0= [file for file in os.listdir(f_tr_0) if file.startswith('sample_0_')]
        if num - len(filelist5_0) < 0:
            new_items5 = sample(filelist5_0, k = len(filelist5_0)- num)
            for img2 in new_items5:
                os.remove(os.path.join(f_tr_0,img2))
        else:
            new_items5 = sample(filelist5, k = num - len(filelist5_0))
            for img2 in new_items5:
                shutil.copy(os.path.join(path_f,img2),os.path.join(f_tr_0,img2))
        print('I completed %s command' %(f_tr_0))
    
    

if __name__ == "__main__":
    main()