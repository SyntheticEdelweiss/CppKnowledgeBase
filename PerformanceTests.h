#pragma once

// result: practically no difference
void ifElseAssignment_vs_lambdaReturnIfElseAssignment();

// see code
void lambdaTypeDeduction();

// result: practically no difference in debug; in release manual assign is 2.5~3 times faster than memset // How is it even possible? Skewed testing methodic?
void memset_vs_manualAssign();

// see code
void lambdaPerformanceTest();
