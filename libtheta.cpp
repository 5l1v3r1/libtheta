#define SKIP_NO_IMPORT
#include <iostream>
#include "common.hpp"

namespace libtheta {

PyObjectRef moments_from_samples(const PyObjectRef &self,
                                 const PyObjectRef &args) {
    PyArrayObject *weights_ = NULL;
    PyArrayObject *samples_ = NULL;

    if (!PyArg_ParseTuple(args.borrow(), "O&O&", const_matrix_converter,
                          &weights_, const_matrix_converter, &samples_)) {
        return nullptr;
    }

    const auto &weights = const_pyvec(newref(weights_));
    const auto &samples = const_pymat(newref(samples_));
    const unsigned dim = samples.rows();
    const unsigned num_samples = samples.cols();
    ENSURE(weights.size() == num_samples);

    Real total_weight = 0.0;
    Vector mean = Vector::Zero(dim);
    Matrix covar_ = Matrix::Zero(dim, dim);
    auto covar = covar_.selfadjointView<Lower>();
    for (unsigned i = 0; i < num_samples; ++i) {
        const auto weight = weights[i];
        const auto sample = samples.col(i);
        mean += weight * sample;
        covar.rankUpdate(sample, weight);
        total_weight += weight;
    }
    mean /= total_weight;
    covar_ /= total_weight;
    covar.rankUpdate(mean, -1);

    const auto &o_mean = pyvec(mean);
    const auto &o_covar = pymat(Matrix(covar));
    return newref(PyTuple_Pack(2, o_mean.borrow(), o_covar.borrow()));
}

PyMethodDef python_methods[] = {
    {"moments_from_samples", pywrap<moments_from_samples>, METH_VARARGS, NULL},
    {0}};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "libtheta",     /* m_name */
    "libtheta",     /* m_doc */
    -1,             /* m_size */
    python_methods, /* m_methods */
    NULL,           /* m_reload */
    NULL,           /* m_traverse */
    NULL,           /* m_clear */
    NULL,           /* m_free */
};

extern "C" PyMODINIT_FUNC initlibtheta() {
    using namespace libtheta;
    import_array();
#if PY_MAJOR_VERSION >= 3
    auto mod = PyModule_Create(&moduledef);
#else
    auto mod = Py_InitModule3("libtheta", python_methods, "libtheta");
#endif
}

}  // namespace libtheta
