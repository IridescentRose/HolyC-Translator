U0 my_function(U32 my_int) {
    if (my_int == 20) {
        "Number is 20!\n";
    } else if (my_int == 19) {
        "Number is 19!\n";
    } else {
        "Number is %d\n", my_int;
    }
}

my_function(18);