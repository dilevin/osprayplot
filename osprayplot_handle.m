classdef osprayplot_handle < matlab.mixin.SetGet
    
    properties (Hidden = true, SetAccess = private)
        cpp_ptr = [];
        current_frame = [];
        
        %materials list for scivis
        materials_scivis = { ...
            'obj' ...
        };
        
        %materials list for pathtracer
        materials_pathtracer = {...
            'obj', ...
            'principled', ...
            'carPaint', ...
            'metal', ...
            'alloy', ...
            'glass', ...
            'thinGlass', ...
            'metallicPaint', ...
            'luminous' ...
       };
   
       lights_pathtracer = {...
            'distant', ...
            'ambient', ...
            'sunSky', ...
            'hdri', ...
            'sphere', ...
            'spot', ...
            'quad' ...
       };
     
    end
    
    properties(SetAccess = public, SetObservable = true)
        
        %main renderer properties 
        RendererMode = 'pathtracer';
        Denoise = 0;  %use ML based denoiser if available;
        BackgroundColor = [1 1 1]'
        NumAverages = 1;
        SamplesPerPixel = 1;
        AmbientOcclusionSamples = 10;
        AmbientOcclusionRadius = 1;
        AmbientOcclusionIntensity = 1;
        RenderFraction = 1; %render frame at a fraction of figure pixel size
        
    end
    
    properties
        dirty = true; %do I need to rerender this frame 
        
        %Object Types
       OSPRAY_RENDERER = int32(0);
       OSPRAY_MATERIAL = int32(1);
       OSPRAY_LIGHT = int32(2);
       
       %handle lists
       Objects = {};
       Lights = {};
       Textures = {};
       
    end
    
    methods
        % Constructor
        function this = osprayplot_handle()
            this.cpp_ptr = osprayplot_mex('new');
            
            renderProps = meta.class.fromName('osprayplot_handle').PropertyList;
            
            for ii=1:numel(renderProps)
                if renderProps(ii).SetObservable
                    addlistener(this, renderProps(ii).Name,'PostSet', @this.rendererUpdates);
                end
            end
             
        end
        
        % Destructor
        function delete(this)
            this.cpp_ptr = osprayplot_mex('del', this.cpp_ptr);
        end
        
         %other methods
         %render the scene if dirty
        function render(this, size_I) 
            if(this.dirty)
                if abs(this.RenderFraction - 1) < sqrt(eps)
                    this.current_frame = osprayplot_mex('render', this.cpp_ptr, size_I.*this.RenderFraction, int32(this.NumAverages), int32(this.Denoise));
                else
                    this.current_frame = imresize(osprayplot_mex('render', this.cpp_ptr, size_I.*this.RenderFraction, int32(this.NumAverages), int32(this.Denoise)), 1/this.RenderFraction, 'method', 'bilinear', 'Antialiasing', true);
                end
                this.dirty = false;
            end
        end
        
        %
        function handle = addObject(this, F,V)
            handle = ospraymeshobj(V,F,[], []); 
            
            this.Objects{numel(this.Objects)+1} = handle;
            
            meshColors = [];
            
            if(size(handle.FaceAlpha,1) == 1)
                meshColors = [handle.FaceVertexCData repmat(handle.FaceAlpha, size(handle.Vertices,1),1)];
            else
                meshColors = [handle.FaceVertexCData handle.FaceAlpha];
            end
            
            handle.ObjectID = osprayplot_mex('addobj', this.cpp_ptr, handle.FaceIndices',...
                                                           handle.Vertices',...
                                                           meshColors');
           
            
            %add listener so I can update the object by changing the handle
            %properties
            handleProps = properties(handle);
             
            for ii=1:numel(handleProps)
                addlistener(handle, handleProps{ii},'PostSet', @this.handleMeshUpdates);
            end
            
            this.dirty = true;
        end
        
        %type is a light type string supported by ospray
        function handle = addLight(this, type)
            handle = ospraylightobj(); 
            
            this.Lights{numel(this.Lights)+1} = handle;
            
            handle.LightID = osprayplot_mex('addlight', this.cpp_ptr, type);  
            this.dirty = true;
        end
        
        function handle = addTexture(this, texture)
            handle = ospraytextureobj(); 
            
            this.Textures{numel(this.Textures)+1} = handle;
            
            handle.TextureID = osprayplot_mex('addtex', this.cpp_ptr, texture);  
            this.dirty = true;
        end
        
        %set camera parameters
        function setCamera(this, pos, target, up, apetureRadius, fov)
            osprayplot_mex('setcam', this.cpp_ptr, pos,target,up, apetureRadius,fov);
            this.dirty = true;
        end
        
        %general methods for setting ospray parameters
        function setParameter(this, objId, objectType, parameterName, value)
            
            if strcmp(class(value), 'ospraytextureobj')  %its a texture
                osprayplot_mex('setpartex', this.cpp_ptr, objId, parameterName, int32(value.TextureID));
            elseif size(value, 2) == 1
                switch class(value)
                    case 'int32'
                        osprayplot_mex('setparint', this.cpp_ptr, objId, objectType, parameterName, value);
                    case 'double'
                        osprayplot_mex('setpardbl', this.cpp_ptr, objId, objectType, parameterName, value);
                end
            elseif size(value, 2) == 3
                switch class(value)
                    case 'int32'
                        osprayplot_mex('setparint', this.cpp_ptr, objId, objectType, parameterName, value');
                    case 'double'
                        osprayplot_mex('setpardbl', this.cpp_ptr, objId, objectType, parameterName, value');
                end
            end
            
            this.dirty = true;
        end
        
        %listeners for various property changes
        function handleMeshUpdates(this, src,evnt)
            
            switch src.Name
          
                case 'Vertices'
                    osprayplot_mex('upmesh', this.cpp_ptr, evnt.AffectedObject.ObjectID, 'vertex.position', evnt.AffectedObject.Vertices');
                case 'FaceIndices'
                    osprayplot_mex('upmesh', this.cpp_ptr, evnt.AffectedObject.ObjectID, 'index', evnt.AffectedObject.FaceIndices');
                case  'FaceVertexCData' 
                    
                    meshColors = [evnt.AffectedObject.FaceVertexCData, ...
                                  ospray_colors(evnt.AffectedObject.FaceAlpha, ...
                                  size(evnt.AffectedObject.Vertices,1))];
                    
                    osprayplot_mex('upmesh', this.cpp_ptr, evnt.AffectedObject.ObjectID, 'vertex.color', meshColors');
                case  'FaceAlpha'
                    
                    meshColors = [evnt.AffectedObject.FaceVertexCData, ...
                                  ospray_colors(evnt.AffectedObject.FaceAlpha, ...
                                  size(evnt.AffectedObject.Vertices,1))];
                    
                    osprayplot_mex('upmesh', this.cpp_ptr, evnt.AffectedObject.ObjectID, 'vertex.color', meshColors');
                    
                case  'VertexNormals'
                    osprayplot_mex('upmesh', this.cpp_ptr, evnt.AffectedObject.ObjectID, 'vertex.normal', evnt.AffectedObject.VertexNormals');
                case 'TextureCoords'
                    osprayplot_mex('upmesh', this.cpp_ptr, evnt.AffectedObject.ObjectID, 'vertex.texcoord', evnt.AffectedObject.TextureCoords');
                case  'Material' 
                    
                    %check if material is compatible
                    if strcmp(this.RendererMode, 'pathtracer')
                        if ~any(strcmp(this.materials_pathtracer, evnt.AffectedObject.Material))
                            evnt.AffectedObject.Material = 'obj';
                            error('Material not supported, switching to obj');
                        end
                    elseif strcmp(this.RendererMode, 'scivis')
                        if ~any(strcmp(this.materials_scivis, evnt.AffectedObject.Material))
                            evnt.AffectedObject.Material = 'obj';
                            error('Material not supported, switching to obj');
                        end
                    end
                    
                    osprayplot_mex('setmat', this.cpp_ptr, evnt.AffectedObject.ObjectID, evnt.AffectedObject.Material);
                case  'FaceColor' 
                    evnt.AffectedObject.FaceVertexCData = ...
                        ospray_colors(evnt.AffectedObject.FaceColor, ...
                        size(evnt.AffectedObject.Vertices,1));
   
            end
            
            this.dirty = true;
        end
        
        function rendererUpdates(this, src,evnt) 
            
            switch src.Name
            
                %RendererMode = 'pathtracer';
                case 'BackgroundColor'
                    setParameter(this, 0, this.OSPRAY_RENDERER, 'backgroundColor', get(this, src.Name));
                    return;
                case 'SamplesPerPixel'
                    setParameter(this, 0, this.OSPRAY_RENDERER, 'pixelSamples', int32(get(this, src.Name)));
                    return
                case 'AmbientOcclusionSamples'
                    setParameter(this, 0, this.OSPRAY_RENDERER, 'aoSamples', int32(get(this, src.Name)));
                    return
                case 'AmbientOcclusionRadius' 
                    setParameter(this, 0, this.OSPRAY_RENDERER, 'aoRadius', get(this, src.Name));
                    return
                case 'AmbientOcclusionIntensity'
                    setParameter(this, 0, this.OSPRAY_RENDERER, 'aoIntensity', get(this, src.Name));
                    return
                case 'NumAverages'
                    this.dirty = true;
                    return
                case 'RenderFraction'
                    this.dirty = true;
                    return
                case 'Denoise'
                    this.Denoise = min(max(0, get(this, src.Name)),1);
                    this.dirty = true;
                    return
            end
       
            fprintf('Property update not supported\n');
            
        end
    end
      
end