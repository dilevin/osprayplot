%handle for an ospray triangle mesh object
%use properties for matlab patch when possible
%stores the properties for a mesh object in an ospray scene
classdef ospraymeshobj < handle

    properties(Access=private)
        dirty = false; %if object properties have changed, rerender
    end
    
    properties(SetObservable)
        ObjectID = []; %don't touch, index into ospray lists
        Vertices = [];
        FaceIndices = [];
        FaceVertexCData = [];
        FaceAlpha = 1; 
        VertexNormals = [];
        TextureCoords =[];
        Material = "obj"; %material name
        FaceColor = [0.6 0.04 0.6];
        
        %all possible material properties here 
    end
    
    methods 
        function this = ospraymeshobj(V,F, C, N) 
            
            if(nargin < 2) 
                error('obspraymeshobj requires at least Vertices (V) and Faces (F) to be initialized');
            end
            
            this.Vertices = V;
            this.FaceIndices = F;
            
            %if no per vertex color data, provided
            %initialize with default color
            if isempty(C)
                this.FaceVertexCData = repmat(this.FaceColor, size(V,1),1);
            else
                this.FaceVertexCData = C;
            end
            
            if ~isempty(N)
                this.VertexNormals = N;
            end
        end
    end
end