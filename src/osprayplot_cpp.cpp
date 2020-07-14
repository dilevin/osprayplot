#include "osprayplot_cpp.h"

//ospray includes
#include <ospray/ospray_cpp.h>

#include <iostream>

using namespace rkcommon::math; 

osprayplot_cpp::osprayplot_cpp() {

    //std::cout<<"Constructor \n";
  
    // initialize OSPRay; OSPRay parses (and removes) its commandline parameters,
    // e.g. "--osp:debug"
    //OSPError init_error = ospInit(&argc, argv);
    
    //initialize ospray renderer (this is nothing like what the documentation says)
    {

      ospLoadModule("ispc");
      m_denoiser_available = true;

      if(ospLoadModule("denoiser") != OSP_NO_ERROR) {
        std::cout<<"Could not load denoiser \n";
        m_denoiser_available = false;
      }

      OSPDevice device = ospNewDevice("cpu");
      ospDeviceCommit(device);
      ospSetCurrentDevice(device);
      
      m_renderer_mode = "pathtracer";

    }
  
}

void osprayplot_cpp::set_ospray_parameter(int obj_id, int object_type, char *param_name, int value) {

  if(object_type == 0) {
    m_renderer.setParam(std::string(param_name), value);
    m_renderer.commit();
  } else if(object_type == 1) {
    m_materials[obj_id].setParam(std::string(param_name), value);
    m_materials[obj_id].commit();
    m_models[obj_id].commit();
    m_instances[obj_id].commit();
  } else if(object_type == 2) {
    m_lights[obj_id].setParam(std::string(param_name), value);
    m_lights[obj_id].commit();
  } else {
    std::cout<<"Object Type Not Supported Yet\n";
  }
}

void osprayplot_cpp::set_ospray_parameter3(int obj_id, int object_type, char *param_name, int *value) {

  vec3ui tmp;
  tmp.x = value[0];
  tmp.y = value[1];
  tmp.z = value[2];

  if(object_type == 0) {
    m_renderer.setParam(std::string(param_name), tmp);
    m_renderer.commit();
  } else if(object_type == 1) {
    m_materials[obj_id].setParam(std::string(param_name), tmp);
    m_materials[obj_id].setParam(std::string(param_name), tmp);
    m_materials[obj_id].commit();
    m_models[obj_id].commit();
    m_instances[obj_id].commit();
  } else if(object_type == 2) {
    m_lights[obj_id].setParam(std::string(param_name), tmp);
    m_lights[obj_id].commit();
  } else {
    std::cout<<"Object Type Not Supported Yet\n";
  }
}

void osprayplot_cpp::set_ospray_parameter(int obj_id, int object_type, char *param_name, double value) {
  
  float tmp = static_cast<float>(value);

  if(object_type == 0) {
    m_renderer.setParam(std::string(param_name), tmp);
    m_renderer.commit();
  } else if(object_type == 1) {
    m_materials[obj_id].setParam(std::string(param_name), tmp);
    m_materials[obj_id].commit();
    m_models[obj_id].commit();
    m_instances[obj_id].commit();
  } else if(object_type == 2) {
      m_lights[obj_id].setParam(std::string(param_name), tmp);
      m_lights[obj_id].commit();
  } else {
    std::cout<<"Object Type Not Supported Yet\n";
  }

}

void osprayplot_cpp::set_ospray_parameter3(int obj_id, int object_type, char *param_name,  double *value) {
  
  vec3f tmp;
  tmp.x = value[0];
  tmp.y = value[1];
  tmp.z = value[2];

  if(object_type == 0) {
    m_renderer.setParam(std::string(param_name), tmp);
    m_renderer.commit();
  } else if(object_type == 1) {
    m_materials[obj_id].setParam(std::string(param_name), tmp);
    m_materials[obj_id].commit();
    m_models[obj_id].commit();
    m_instances[obj_id].commit();
  } else if(object_type == 2) {
      m_lights[obj_id].setParam(std::string(param_name), tmp);
      m_lights[obj_id].commit();
  } else {
    std::cout<<"Object Type Not Supported Yet\n";
  }
}

void osprayplot_cpp::set_ospray_parameter_texture(int obj_id, char * param_name, int tex_id) {
  
  if(tex_id >= m_textures.size()) {
    std::cout<<"Incorrect texture id \n";
    return;
  }

  //std::cout<<obj_id<<" "<<tex_id<<" "<<param_name<<"\n";
  m_materials[obj_id].setParam(std::string(param_name), m_textures[tex_id]);
  m_materials[obj_id].commit();
  m_models[obj_id].commit();
  m_instances[obj_id].commit();
  //OSPDevice dev = ospGetCurrentDevice();
  //std::cout<<"1: "<<ospDeviceGetLastErrorMsg(dev)<<"\n";
  //ospDeviceRelease(dev);

}

void osprayplot_cpp::setup_default_scene() {

  //setup default params for things
  {
    m_camera = ospray::cpp::Camera("perspective");
    m_camera.setParam("position", vec3f(0.f));
    m_camera.setParam("direction", vec3f(0.1f, 0.f, 1.f));
    m_camera.setParam("up", vec3f(0.f, 1.f, 0.f));

    m_world = ospray::cpp::World();

    // create renderer, choose Scientific Visualization renderer
    m_renderer = ospray::cpp::Renderer(m_renderer_mode);

    // complete setup of renderer
    m_renderer.setParam("aoSamples", 10);
    m_renderer.setParam("pixelSamples", 1);
    m_renderer.setParam("backgroundColor", 1.0f); // white, transparent
  }

}

int osprayplot_cpp::add_object(double *F, size_t num_faces, double *V, size_t num_vertices, double *C) {

  int obj_id = m_positions.size();

  //copy over vertices
  m_positions.push_back(std::vector<vec3f>(num_vertices));
  for(unsigned int ii=0; ii<3*num_vertices; ii+=3) {
    m_positions.back()[ii/3].x = V[ii+0];
    m_positions.back()[ii/3].y = V[ii+1];
    m_positions.back()[ii/3].z = V[ii+2];
  }

  //copy over colors
  m_colors.push_back(std::vector<vec4f>(num_vertices));
  for(unsigned int ii=0; ii<4*num_vertices; ii+=4) {
    m_colors.back()[ii/4].x = C[ii+0];
    m_colors.back()[ii/4].y = C[ii+1];
    m_colors.back()[ii/4].z = C[ii+2];
    m_colors.back()[ii/4].w = C[ii+3];
  }

  //copy over faces
  m_indices.push_back(std::vector<vec3ui>(num_faces));
  for(unsigned int ii=0; ii<3*num_faces; ii+=3) {
    m_indices.back()[ii/3].x = static_cast<unsigned int>(F[ii+0])-1;
    m_indices.back()[ii/3].y = static_cast<unsigned int>(F[ii+1])-1;
    m_indices.back()[ii/3].z = static_cast<unsigned int>(F[ii+2])-1;
  }

  //start with no normals put allocate space for later
  m_normals.push_back(std::vector<vec3f>(num_vertices));

  m_texcoords.push_back(std::vector<vec2f>(num_vertices));

  // create and setup model and mesh
  m_geometries.push_back(ospray::cpp::Geometry("mesh"));
  m_geometries.back().setParam("vertex.position", ospray::cpp::Data(m_positions.back()));
  m_geometries.back().setParam("vertex.color", ospray::cpp::Data(m_colors.back()));
  m_geometries.back().setParam("index", ospray::cpp::Data(m_indices.back()));
  m_geometries.back().commit();

  //obj material is supported by both renderers.
  m_materials.push_back(ospray::cpp::Material(m_renderer_mode, "obj"));
  m_materials.back().commit();
  
  // put the mesh into a model
  m_models.push_back(ospray::cpp::GeometricModel(m_geometries.back()));
  m_models.back().setParam("material", ospray::cpp::Data(m_materials.back()));
  m_models.back().commit();


  // put the model into a group (collection of models)
  m_groups.push_back(ospray::cpp::Group());
  m_groups.back().setParam("geometry", ospray::cpp::Data(m_models.back()));
  m_groups.back().commit();

  // put the group into an instance (give the group a world transform)
  m_instances.push_back(ospray::cpp::Instance(m_groups.back()));
  m_instances.back().commit();

  m_world.setParam("instance", ospray::cpp::Data(m_instances));

  return obj_id;

}

int osprayplot_cpp::add_light(char *type) {
    
    int light_id = m_lights.size();

    m_lights.push_back(ospray::cpp::Light(std::string(type)));
    m_lights.back().commit();

    m_world.setParam("light", ospray::cpp::Data(m_lights));

    return light_id;

}

int osprayplot_cpp::add_texture_2d(int height, int width, int channels, double *data) {

  int tex_id = m_textures.size();

  m_texture_data.push_back(std::vector<vec4f>(height*width));

  //std::cout<<height<<" "<<width<<" "<<channels<<"\n";
  for(unsigned int ii=0; ii<height; ++ii) {
    for(unsigned int jj=0; jj<width; ++jj) {
        vec4f tmp; tmp.x=tmp.y=tmp.z=tmp.w=1.f;

        tmp.x = data[ii + jj*height + 0*height*width];
        tmp.y = data[ii + jj*height + 1*height*width];
        tmp.z = data[ii + jj*height + 2*height*width];
        tmp.w = (channels == 4 ? data[ii + jj*height + 3*height*width] : 1.f);

        //std::cout<<tmp<<"\n";
        m_texture_data.back()[jj + ii*width] = tmp;
      }
    }

  m_textures.push_back(ospray::cpp::Texture("texture2d"));
  m_textures.back().setParam("format", OSP_TEXTURE_RGBA32F);
  m_textures.back().setParam("filter", OSP_TEXTURE_FILTER_BILINEAR);
  //OSPDevice dev = ospGetCurrentDevice();
  //std::cout<<"1: "<<ospDeviceGetLastErrorMsg(dev)<<"\n";
  //ospDeviceRelease(dev);
  

  vec2ul tex_size;
  tex_size.x = width;
  tex_size.y = height;
  
  //std::cout<<"SIZE: "<<width<<" "<<height<<"\n";
  m_textures.back().setParam("data", ospray::cpp::CopiedData((vec4f *)(m_texture_data.back().data()), tex_size));
  //dev = ospGetCurrentDevice();
  //std::cout<<ospDeviceGetLastErrorMsg(dev)<<"\n";
  
  //ospDeviceRelease(dev);
  m_textures.back().commit();

  return tex_id;
}

void osprayplot_cpp::set_camera(double *pos, double *target, double *up, double apeture_radius, double fovy) {

    float fovy_f = fovy;
    float apeture_radius_f = apeture_radius;

    m_camera.setParam("position", vec3f(pos[0], pos[1], pos[2]));
    m_camera.setParam("direction", vec3f(target[0], target[1], target[2]));
    m_camera.setParam("up", vec3f(up[0], up[1], up[2]));
    m_camera.setParam("fovy", fovy_f);
    m_camera.setParam("apetureRadius", apeture_radius_f);
}

void osprayplot_cpp::set_material(int objId, char *material_name) {

  m_materials[objId] = ospray::cpp::Material(m_renderer_mode, std::string(material_name));
  m_materials[objId].commit();
  
  m_models[objId].setParam("material", ospray::cpp::Data(m_materials[objId]));
  m_models[objId].commit();
  m_instances[objId].commit();
  
}

void osprayplot_cpp::update_vertex_params(int objId, char *property, double *data, int num_data) {

  if(!strcmp(property,"vertex.position")) {
    
    m_positions[objId].resize(num_data);
    
    for(unsigned int ii=0; ii<3*num_data; ii+=3) {
      m_positions[objId][ii/3].x = data[ii+0];
      m_positions[objId][ii/3].y = data[ii+1];
      m_positions[objId][ii/3].z = data[ii+2];
    }

    m_geometries[objId].setParam("vertex.position", ospray::cpp::Data(m_positions[objId]));

  } else if(!strcmp(property,"vertex.color")) {

    m_colors[objId].resize(num_data);
    
    for(unsigned int ii=0; ii<4*num_data; ii+=4) {
      m_colors[objId][ii/4].x = data[ii+0];
      m_colors[objId][ii/4].y = data[ii+1];
      m_colors[objId][ii/4].z = data[ii+2];
      m_colors[objId][ii/4].w = data[ii+3];
    }
    
    m_geometries[objId].setParam("vertex.color", ospray::cpp::Data(m_colors[objId]));

  } else if(!strcmp(property,"vertex.texcoord")) {

    
    m_texcoords[objId].resize(num_data);
    
    //std::cout<<"NUMDATA: "<<num_data<<"\n";

    for(unsigned int ii=0; ii<2*num_data; ii+=2) {
      m_texcoords[objId][ii/2].x = data[ii+0];
      m_texcoords[objId][ii/2].y = data[ii+1];

      //std::cout<<m_texcoords[objId][ii/2]<<"\n";

    }

    m_geometries[objId].setParam("vertex.texcoord", ospray::cpp::CopiedData(m_texcoords[objId]));
    
  } else if(!strcmp(property,"index")) {

    m_indices[objId].resize(num_data);
    
    for(unsigned int ii=0; ii<3*num_data; ii+=3) {
      m_indices[objId][ii/3].x = static_cast<unsigned int>(data[ii+0]);
      m_indices[objId][ii/3].y = static_cast<unsigned int>(data[ii+1]);
      m_indices[objId][ii/3].z = static_cast<unsigned int>(data[ii+2]);
    }


    m_geometries[objId].setParam("index", ospray::cpp::Data(m_indices[objId]));

  } else if(!strcmp(property, "vertex.normal")) {

      //turn off normals
      if(num_data == 0) {
        m_geometries[objId].removeParam("vertex.normal");
      } else {

        m_normals[objId].resize(num_data);
    
        for(unsigned int ii=0; ii<3*num_data; ii+=3) {
          m_normals[objId][ii/3].x = data[ii+0];
          m_normals[objId][ii/3].y = data[ii+1];
          m_normals[objId][ii/3].z = data[ii+2];
        }

        m_geometries[objId].setParam("vertex.normal", ospray::cpp::Data(m_normals[objId]));
      }

  }

  
  
  m_geometries[objId].commit();
  m_groups[objId].commit();
  m_instances[objId].commit();

}

void osprayplot_cpp::render(int height, int width, char *image_buffer, int num_averages, bool denoise) {

    // image size
    vec2i imgSize;
    imgSize.x = width; // width
    imgSize.y = height; // height
    
    {
      m_camera.setParam("aspect", (float)imgSize.x/(float)imgSize.y);

      m_camera.commit(); 
      m_world.commit();
      m_renderer.commit();
    
      // create and setup light for Ambient Occlusion
      
      
      // create and setup framebuffer
      //ospray::cpp::FrameBuffer framebuffer(imgSize, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_ACCUM);
      //ospray::cpp::FrameBuffer framebuffer(imgSize, OSP_FB_RGBA32F, OSP_FB_COLOR);

      auto buffers = OSP_FB_COLOR | OSP_FB_DEPTH | OSP_FB_ACCUM | OSP_FB_ALBEDO | OSP_FB_NORMAL;
  
      ospray::cpp::FrameBuffer framebuffer = ospray::cpp::FrameBuffer(imgSize.x, imgSize.y, OSP_FB_RGBA32F, buffers);

      if(m_denoiser_available && denoise) {
        //turn on DNN denoiser
        ospray::cpp::ImageOperation d("denoiser");
        framebuffer.setParam("imageOperation", ospray::cpp::Data(d));
        framebuffer.commit();
      }

      framebuffer.clear();

      // render 10 more frames, which are accumulated to result in a better
      // converged image
      for (int frames = 0; frames < num_averages; frames++)
        framebuffer.renderFrame(m_renderer, m_camera, m_world);

      float * fb = (float *)framebuffer.map(OSP_FB_COLOR);
      
      for (int k = 0; k < 3; k++) {
          for (int i = 0; i < height; i++) { 
              for (int j = 0; j < width; j++) {
                  float pixel_val = 255.f*std::min(((float *)&fb[4*(imgSize.y-1-i)*imgSize.x + 4*j])[k],1.f);
                  //char pixel_val = ((char *)&fb[4*(imgSize.y-1-i)*imgSize.x + 4*j])[k];
                  image_buffer[k*height*width + i + j*height] = pixel_val;
              }
          }
      }

      framebuffer.unmap(fb);

    }

}

void osprayplot_cpp::shutdown() {

    {
      ospShutdown();
    }
}

osprayplot_cpp::~osprayplot_cpp() {

    //std::cout<<"Destructor \n"; 

}