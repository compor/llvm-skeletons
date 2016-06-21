; RUN: opt -load %bindir/libLLVMSkeletonOptPass${MOD_EXT} -skeleton-opt-pass %s -S -o %t0.ll
; RUN: opt -load %bindir/libLLVMSkeletonOptPass${MOD_EXT} -skeleton-opt-pass %s -S | FileCheck %s

; CHECK-LABEL: foo
; CHECK: [[REGISTER:%i]] = alloca [[TYPE:i[1-9][0-9]*]]
; CHECK: store [[TYPE]] {{[0-9]+}}, [[TYPE]]* [[REGISTER]]

define i32 @foo(i32 %a) {
  %1 = alloca i32, align 4
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %a, i32* %1, align 4
  store i32 0, i32* %k, align 4
  store i32 1, i32* %i, align 4
  br label %2

; <label>:2                                       ; preds = %9, %0
  %3 = load i32, i32* %i, align 4
  %4 = icmp slt i32 %3, 10
  br i1 %4, label %5, label %12

; <label>:5                                       ; preds = %2
  %6 = load i32, i32* %1, align 4
  %7 = load i32, i32* %k, align 4
  %8 = add nsw i32 %7, %6
  store i32 %8, i32* %k, align 4
  br label %9

; <label>:9                                       ; preds = %5
  %10 = load i32, i32* %i, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, i32* %i, align 4
  br label %2

; <label>:12                                      ; preds = %2
  %13 = load i32, i32* %k, align 4
  ret i32 %13
}

