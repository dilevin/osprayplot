#include <mex.h>
#include "osprayplot_cpp.h"

inline osprayplot_cpp *get_ptr(const mxArray *in)
{
    
    if (mxGetNumberOfElements(in) != 1 || mxGetClassID(in) != mxUINT64_CLASS || mxIsComplex(in))
        mexErrMsgTxt("Input must be a real uint64 scalar.");

    return reinterpret_cast<osprayplot_cpp *>(*((uint64_t *)mxGetData(in)));
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

    char cmd[64];
    
    //Get command string
    if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
		mexErrMsgTxt("First input should be a command string less than 64 characters long.");

    //New object
     if (!strcmp("new", cmd)) {
        
        // Check parameters
        if (nlhs != 1) {
            mexErrMsgTxt("New: One output expected.");
            return;
        }
        
        // Return a handle to a new C++ instance
        osprayplot_cpp *ptr = new osprayplot_cpp();
        ptr->setup_default_scene();
        //mexLock(); 
        plhs[0] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
        *((uint64_t *)mxGetData(plhs[0])) = reinterpret_cast<uint64_t>(ptr);
        //mexUnlock();
        return;
     }

    if(!strcmp("del", cmd)) {
        
        if (nlhs != 1 || nrhs != 2) {
            mexErrMsgTxt("Delete: one output, one input expected.");
            return; 
        }

        //mexLock();
        osprayplot_cpp *ptr = get_ptr(prhs[1]);
        plhs[0] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
        *((uint64_t *)mxGetData(plhs[0])) = 0;
        //ptr->shutdown();
        delete ptr; 
        //mexUnlock(); 
        return;

    }

    if(!strcmp("addobj", cmd)) {
        if (nlhs != 1 || nrhs != 5) {
            mexErrMsgTxt("Render: one output, 4 inputs expected.");
            return; 
        }

        size_t num_faces = mxGetN(prhs[2]);
        size_t num_vertices = mxGetN(prhs[3]);
    
        //std::cout<<"Num Faces: "<<num_faces<<"\n";
        //std::cout<<"Num Vertices: "<<num_vertices<<"\n";

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        int objid = ptr->add_object(mxGetPr(prhs[2]), num_faces, mxGetPr(prhs[3]), num_vertices, mxGetPr(prhs[4]));

        plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
        ((int *)mxGetData(plhs[0]))[0] = objid;
        
    }

    if(!strcmp("addlight", cmd)) {
        if (nlhs != 1 || nrhs != 3) {
            mexErrMsgTxt("Render: one output, 1 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        char light_type[256];
        mxGetString(prhs[2], light_type, sizeof(light_type));


        int light_id = ptr->add_light(light_type);

        plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
        ((int *)mxGetData(plhs[0]))[0] = light_id;
        
    }

    if(!strcmp("addtex", cmd)) {

        
        if (nlhs != 1 || nrhs != 3) {
            mexErrMsgTxt("Render: one output, 1 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);
        
        const mwSize * pDims = mxGetDimensions(prhs[2]);

        plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
        int tex_id = ptr->add_texture_2d(pDims[0], pDims[1], pDims[2], mxGetPr(prhs[2]));
        
        ((int *)mxGetData(plhs[0]))[0] = tex_id;
        
    }

    if(!strcmp("setcam", cmd)) {
        if (nlhs != 0 || nrhs != 7) {
            mexErrMsgTxt("SetCam: one output, 5 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        ptr->set_camera(mxGetPr(prhs[2]), 
                        mxGetPr(prhs[3]), 
                        mxGetPr(prhs[4]), 
                        mxGetPr(prhs[5])[0],
                        mxGetPr(prhs[6])[0]);

    }

    if(!strcmp("upmesh", cmd)) {
        if (nlhs != 0 || nrhs != 5) {
            mexErrMsgTxt("Update Mesh: no output, 3 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        int obj_id= ((int*)mxGetData(prhs[2]))[0];
        
        char property[256];
        mxGetString(prhs[3], property, sizeof(property));

        size_t num_data = mxGetN(prhs[4]);

        //std::cout<<obj_id<<" "<<property<<" "<<num_data<<"\n";
        ptr->update_vertex_params(obj_id,  property, mxGetPr(prhs[4]), num_data);


    }

    if(!strcmp("setmat", cmd)) {
        if (nlhs != 0 || nrhs != 4) {
            mexErrMsgTxt("Set Material: no output, 2 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        int obj_id= ((int*)mxGetData(prhs[2]))[0];
        
        char material[256];
        mxGetString(prhs[3], material, sizeof(material));

        //std::cout<<obj_id<<" "<<material<<"\n";
        ptr->set_material(obj_id, material);


    }

    if(!strcmp("setparint", cmd)) {
        if (nlhs != 0 || nrhs != 6) {
            mexErrMsgTxt("Set Material: no output, 4 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        int obj_id= ((int*)mxGetData(prhs[2]))[0];
        int obj_type= ((int*)mxGetData(prhs[3]))[0];
        
        char param_name[256];
        mxGetString(prhs[4], param_name, sizeof(param_name));

        size_t data_size = mxGetM(prhs[5]);

        if(data_size == 1) {
            ptr->set_ospray_parameter(obj_id, obj_type, param_name, ((int*)mxGetData(prhs[5]))[0]);
        } else if(data_size == 3) {
            ptr->set_ospray_parameter3(obj_id, obj_type, param_name, (int*)mxGetData(prhs[5]));
        } else {
            std::cout<<"Data size not supported\n";
        }

    }

    if(!strcmp("setpardbl", cmd)) {
        if (nlhs != 0 || nrhs != 6) {
            mexErrMsgTxt("Set Material: no output, 4 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        int obj_id= ((int*)mxGetData(prhs[2]))[0];
        int obj_type= ((int*)mxGetData(prhs[3]))[0];
        
        char param_name[256];
        mxGetString(prhs[4], param_name, sizeof(param_name));

        size_t data_size = mxGetM(prhs[5]);

        if(data_size == 1) {
            ptr->set_ospray_parameter(obj_id, obj_type, param_name, ((double*)mxGetData(prhs[5]))[0]);
        } else if(data_size == 3) {
            ptr->set_ospray_parameter3(obj_id, obj_type, param_name, (double*)mxGetData(prhs[5]));
        } else {
            std::cout<<"Data size not supported\n";
        }

    }
    
    if(!strcmp("setpartex", cmd)) {
        if (nlhs != 0 || nrhs != 5) {
            mexErrMsgTxt("Set Material: no output, 3 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        int obj_id= ((int*)mxGetData(prhs[2]))[0];
        
        char param_name[256];
        mxGetString(prhs[3], param_name, sizeof(param_name));

        int tex_id= ((int*)mxGetData(prhs[4]))[0];
        
        ptr->set_ospray_parameter_texture(obj_id, param_name, tex_id);

    }
    if(!strcmp("render", cmd)) {

        if (nlhs != 1 || nrhs != 5) {
            mexErrMsgTxt("Render: one output, 3 inputs expected.");
            return; 
        }

        osprayplot_cpp *ptr = get_ptr(prhs[1]);

        mwSize im_size[3];

        im_size[0] = static_cast<unsigned int>(mxGetPr(prhs[2])[0]);
        im_size[1] = static_cast<unsigned int>(mxGetPr(prhs[2])[1]);
        im_size[2] = 3;

        int num_averages = ((int *)mxGetData(prhs[3]))[0];
        int denoise = ((int *)mxGetData(prhs[4]))[0];

        plhs[0] = mxCreateNumericArray(3, im_size, mxUINT8_CLASS, mxREAL);
        
        ptr->render(static_cast<unsigned int>(mxGetPr(prhs[2])[0]),
                    static_cast<unsigned int>(mxGetPr(prhs[2])[1]),
                    (char *)mxGetData(plhs[0]), num_averages, (denoise == 1));
        
    }
        

}	
