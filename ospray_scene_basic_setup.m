%f - a figure displaying a mesh
%output an ospray figure with scene setup
function f_ospray = ospray_scene_basic_setup(f)

    if nargin < 1
        f = gcf;
    end
    
    a = f.CurrentAxes;
    
    %create ospray figure
    f_ospray = figure;
    
    hold on;
    
    for i=numel(a.Children):-1:1
        
        if contains(class(a.Children(i)), 'Patch')
            
            if size(a.Children(i).Faces, 2) == 3
                p = ospraytsurf(a.Children(i).Faces, a.Children(i).Vertices, f_ospray);
                p.Material = 'principled';
                p.FaceColor = [1, 0.537, 0.039];
                f_ospray.ospray.setParameter(p.ObjectID, f_ospray.ospray.OSPRAY_MATERIAL, 'metallic', 0.3); 
                f_ospray.ospray.setParameter(p.ObjectID, f_ospray.ospray.OSPRAY_MATERIAL, 'sheen', 0.5); 
                f_ospray.ospray.setParameter(p.ObjectID, f_ospray.ospray.OSPRAY_MATERIAL, 'roughness', 0.75); 
            end
            
        end
    end
    
    hold off;
    
    if ~isprop(f_ospray, 'ospray')
        error('No triangle patches found in figure, no ospray renderer created');
        return;
    end
    
    %set camera params to match f
    v = a.CameraUpVector;
    v = v./norm(v);
    w = -(a.CameraTarget-a.CameraPosition)./norm(a.CameraTarget-a.CameraPosition);
    u = cross(w,v);
    u = u/norm(u);
    
    f_ospray.ospray.Denoise = 1;
    f_ospray.ospray.SamplesPerPixel = 4;
    f_ospray.ospray.setCamera(a.CameraPosition, -w, v, 0.1, a.CameraViewAngle);
    f_ospray.Position = f.Position;
    
    %use my basic scene setup of sunSky light and ambient light 
    l0 = ospraylight('sunSky');
    l1 = ospraylight('ambient');
    
    f_ospray.ospray.setParameter(l1.LightID, f_ospray.ospray.OSPRAY_LIGHT, 'intensity', 1.0); 
    f_ospray.ospray.setParameter(l0.LightID, f_ospray.ospray.OSPRAY_LIGHT, 'intensity', 0.8); 
    f_ospray.ospray.setParameter(l0.LightID, f_ospray.ospray.OSPRAY_LIGHT, 'up', v); 
    f_ospray.ospray.setParameter(l0.LightID, f_ospray.ospray.OSPRAY_LIGHT, 'direction', -v + u - w); 

    %drawnowOspray(f_ospray);
    
end