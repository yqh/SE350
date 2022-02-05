The Project Automated Testing Framework has the following source Groups:

AE:     
    It contains auto-tester general purpose routines. 
    This source group is included in Target build.
    You may change the implementation as long as you keep the interface of functions unchanged. 
    The files are:    
    - ae.c: the auto-tester initialization and exiting routines
    - ae_util.c: the auto-tester utility functions.

AE-active
    It contains the active testing suite that you want to use.
    By default It contains ae_proc.c which is an initial dummy test suite written by lab teaching team to get starter code working.
    This source group is included in Target build.
    
    To use a test suite .c file from AE-pub-inactive or AE-stu-inactive 
    - Drag the single .c file under AE-active to one of the AE-*-inactive folders.
    - Drag the corresponding test suite .c file to the AE-active folder.

AE-pub-inactive: 
    It contains public testing suites released by lab staff. By default, this source group is excluded from Target build.
    Each testing suit is in a separate .c file.
    The naming convention of the public testing suite is ae_proc<suite ID>.c 
    For example:
    ae_proc100.c is Test Suite 100.
    
    Teaching team reserves the following test suite IDs:
    
    Test suite IDs 100 - 199 are for P1
    Test suite IDs 200 - 299 are for P2
    Test suite IDs 300 - 399 are for P3
    
AE-stu-inactive:
    It contains students' testing suites. By default, this source group is excluded from Target build..
    Each testing suit is in a separate .c file. 
    The naming convention is ae_proc<suite ID>.c.  For example
    ae_proc1.c is the Test Suite 1 done by the student.
    
    Student test suite IDs should be in the range of 1-99
    By default, we put three test suite template files in the starter code.
    The minimum requirement is three test cases. You may put them into one test suite or separate them into different test suites.

IMPORTANT    
    When submit, you should 
    
    - leave the AE-active folder with the starter code ae_proc.c and make sure the project builds without errors.
    - include the following groups in Target build
        * AE
        * AE-active
    - exclude the following groups from Target build
        * AE-public
        * AE-stu
	
    - You should disable DEBUG_0 and SE350_DEMO macros in both SIM and RAM target option setting.
      The starter code has these two macros enabled to demonstrate more information.
    - To use the provided ae_util.c functions, AE_ENABLE needs to be defined.
        