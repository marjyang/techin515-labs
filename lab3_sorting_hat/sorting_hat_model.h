#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class DecisionTree {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        if (x[3] <= 1.5) {
                            return 0;
                        }

                        else {
                            if (x[4] <= 2.5) {
                                if (x[0] <= 1.5) {
                                    if (x[6] <= 3.0) {
                                        return 0;
                                    }

                                    else {
                                        return 2;
                                    }
                                }

                                else {
                                    return 1;
                                }
                            }

                            else {
                                if (x[9] <= 3.5) {
                                    return 2;
                                }

                                else {
                                    if (x[7] <= 2.5) {
                                        if (x[0] <= 3.0) {
                                            return 2;
                                        }

                                        else {
                                            return 0;
                                        }
                                    }

                                    else {
                                        return 3;
                                    }
                                }
                            }
                        }
                    }

                protected:
                };
            }
        }
    }