
define void @x(i32 %ht, %"%struct._zval_struct"* %return_value, %"%struct._zval_struct"** %return_value_ptr, %"%struct._zval_struct"* %this_ptr, i32 %return_value_used) {
entry:
  %0 = alloca i32
  %1 = alloca %"%struct._zval_struct"*
  %2 = alloca %"%struct._zval_struct"**
  %3 = alloca %"%struct._zval_struct"*
  %4 = alloca i32
  store i32 %ht, i32* %0
  store %"%struct._zval_struct"* %return_value, %"%struct._zval_struct"** %1
  store %"%struct._zval_struct"** %return_value_ptr, %"%struct._zval_struct"*** %2
  store %"%struct._zval_struct"* %this_ptr, %"%struct._zval_struct"** %3
  store i32 %return_value_used, i32* %4
  %accuracy_param = alloca %"%struct._zval_struct"*
  %accuracy = alloca i64
  %5 = call i32 (i32, i32, i32, ...)* @zephir_fetch_parameters(i32 1, i32 1, i32 0, %"%struct._zval_struct"** %accuracy_param)
  %6 = icmp eq i32 %5, -1
  br i1 %6, label %then, label %else

then:                                             ; preds = %entry
  ret void

else:                                             ; preds = %entry
  br label %merge-if

merge-if:                                         ; preds = %else
  %7 = load %"%struct._zval_struct"** %accuracy_param
  %8 = getelementptr inbounds %"%struct._zval_struct"* %7, i32 0, i32 2
  %9 = load i8* %8
  %10 = zext i8 %9 to i32
  %11 = icmp eq i32 %10, 1
  br i1 %11, label %then1, label %else2

then1:                                            ; preds = %merge-if
  %12 = load %"%struct._zval_struct"** %accuracy_param
  %13 = getelementptr inbounds %"%struct._zval_struct"* %12, i32 0, i32 0
  %14 = bitcast %"%union._zvalue_value"* %13 to i64*
  %15 = load i64* %14
  br label %merge-if3

else2:                                            ; preds = %merge-if
  %16 = load %"%struct._zval_struct"** %accuracy_param
  %17 = call i64 @zephir_get_intval_ex(%"%struct._zval_struct"* %16)
  br label %merge-if3

merge-if3:                                        ; preds = %else2, %then1
  %18 = phi i64 [ %15, %then1 ], [ %17, %else2 ]
  store i64 %18, i64* %accuracy
  %i = alloca i64
  %pi = alloca double
  store double 4.000000e+00, double* %pi
  %top = alloca double
  store double 4.000000e+00, double* %top
  %bottom = alloca double
  store double 3.000000e+00, double* %bottom
  %minus = alloca i8
  store i8 1, i8* %minus
  store i64 0, i64* %i
  br label %start-while

start-while:                                      ; preds = %merge-if6, %merge-if3
  %19 = load i64* %i
  %20 = load i64* %accuracy
  %21 = icmp slt i64 %19, %20
  br i1 %21, label %block-while, label %merge-while

block-while:                                      ; preds = %start-while
  %22 = load i8* %minus
  %23 = icmp ne i8 %22, 0
  br i1 %23, label %then4, label %else5

merge-while:                                      ; preds = %start-while
  %24 = load double* %pi
  %25 = load %"%struct._zval_struct"** %1
  %26 = getelementptr inbounds %"%struct._zval_struct"* %25, i32 0, i32 2
  store i8 2, i8* %26
  %27 = load %"%struct._zval_struct"** %1
  %28 = getelementptr inbounds %"%struct._zval_struct"* %27, i32 0, i32 0
  %29 = bitcast %"%union._zvalue_value"* %28 to double*
  store double %24, double* %29
  ret void

then4:                                            ; preds = %block-while
  %30 = load double* %top
  %31 = load double* %bottom
  %32 = fdiv double %30, %31
  %33 = fmul double %32, -1.000000e+00
  %34 = load double* %pi
  %35 = fadd double %34, %33
  store double %35, double* %pi
  store i8 0, i8* %minus
  br label %merge-if6

else5:                                            ; preds = %block-while
  %36 = load double* %top
  %37 = load double* %bottom
  %38 = fdiv double %36, %37
  %39 = load double* %pi
  %40 = fadd double %39, %38
  store double %40, double* %pi
  store i8 1, i8* %minus
  br label %merge-if6

merge-if6:                                        ; preds = %else5, %then4
  %41 = load double* %bottom
  %42 = fadd double %41, 2.000000e+00
  store double %42, double* %bottom
  %43 = load i64* %i
  %44 = add nsw i64 %43, 1
  store i64 %44, i64* %i
  br label %start-while
}

