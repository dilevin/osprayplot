#include <iostream>
#include <vector>

#include <ospray/ospray_cpp.h>

class osprayplot_cpp 
{

public:
    osprayplot_cpp();
    ~osprayplot_cpp();

    //void set_mesh();

    //should fill in a buffer from matlab
    void render(int height, int width, char *image_buffer, int num_averages, bool denoise);
    
    void setup_default_scene();
    
    int add_object(double *F, size_t num_faces, double *V, size_t num_vertices, double *C);

    int add_light(char *type);

    //add a 2d textture stored as either RGB8 or RGBQ8
    int add_texture_2d(int height, int width, int channels, double *data);

    void set_camera(double *pos, double *target, double *up, double apeture_radius, double fovy);

    void set_material(int obj_id, char *material_name);

    //object_type:
    //0 = renderer
    //1 = material
    //2 = light
    void set_ospray_parameter(int obj_id, int object_type, char *param_name, int value);

    void set_ospray_parameter(int obj_id, int object_type, char *param_name, double value);

    void set_ospray_parameter3(int obj_id, int object_type, char *param_name,  int *value);

    void set_ospray_parameter3(int obj_id, int object_type, char *param_name,  double *value);

    void set_ospray_parameter_texture(int obj_id, char *param_name, int tex_id);

    //update parameters once the scene is setup
    void  update_vertex_params(int objId, char *property, double *data, int num_data);

    void shutdown();

private:

protected:

    //Camera
    ospray::cpp::Camera m_camera{nullptr};
    
    
    //World
    ospray::cpp::World m_world{nullptr};

    //Renderer
    ospray::cpp::Renderer m_renderer{nullptr};

    //Frame Buffer
    ospray::cpp::FrameBuffer m_framebuffer{nullptr};
    OSPFrameBufferFormat m_frameBufferFormat = OSP_FB_SRGBA;

    //Objects
    std::vector<ospray::cpp::Group> m_groups; //one group per instance, one object per group
    std::vector<ospray::cpp::GeometricModel> m_models; //one group per instance, one object per group
    std::vector<ospray::cpp::Material> m_materials; //one group per instance, one object per group
    std::vector<ospray::cpp::Geometry> m_geometries; //one group per instance, one object per group
    std::vector<ospray::cpp::Instance> m_instances; //one group per instance, one object per group

    //raw data for conversions
    std::vector<std::vector<ospcommon::math::vec3f> > m_positions;
    std::vector<std::vector<ospcommon::math::vec4f> > m_colors;
    std::vector<std::vector<ospcommon::math::vec3ui> > m_indices;
    std::vector<std::vector<ospcommon::math::vec3f> > m_normals;
    std::vector<std::vector<ospcommon::math::vec2f> > m_texcoords;

    //List of Lights
    std::vector<ospray::cpp::Light> m_lights;

    //List of Textures
    std::vector<ospray::cpp::Texture> m_textures;
    std::vector<std::vector<ospcommon::math::vec4f>> m_texture_data;

    std::string m_renderer_mode;

    bool m_denoiser_available = false;

    //data arrays

    //Geometry -> Model -> Group -> Instance

    //set transforms on the instances
    //set materials on the model
    //set geometry data on the geometry 
    //set param on any of these 

    //image tool stuff
    //std::unique_ptr<ospray::cpp::OSPImageTools> imageTool;

};