%handle for an ospray triangle mesh object
%use properties for matlab patch when possible
%stores the properties for a mesh object in an ospray scene
classdef ospraylightobj < handle

    properties(SetObservable)
        LightID = []; %don't touch, index into ospray lists
    end
    
    methods 
        function this = ospraylightobj() 
            
        end
    end
end