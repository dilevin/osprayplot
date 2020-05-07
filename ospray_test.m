f = figure; p = ospraytsurf(F,V);
f.ospray.Denoise = 1;
f.ospray.NumAverages = 2;
f.ospray.SamplesPerPixel = 16;

hold on; p1 = ospraytsurf(F0, V0-[0 0 2.5]); 
p1.Vertices = [-2 -2 -0.5; 2 -2 -0.5; 2 2 -0.5; -2 2 -0.5];
p1.FaceColor = [1.5 1.5 1.5]; 


l0 = ospraylight('sunSky');
l1 = ospraylight('ambient');
%l2 = ospraylight('distant')
% f.ospray.setParameter(l2.LightID, f.ospray.OSPRAY_LIGHT, 'color', [1 1 1]); 
f.ospray.setParameter(l1.LightID, f.ospray.OSPRAY_LIGHT, 'intensity', 0.6); 
f.ospray.setParameter(l0.LightID, f.ospray.OSPRAY_LIGHT, 'intensity', 0.65); 
% f.ospray.setParameter(l2.LightID, f.ospray.OSPRAY_LIGHT, 'direction', [0 0 -1]); 
hold off;

f.ospray.setParameter(l0.LightID, f.ospray.OSPRAY_LIGHT, 'up', [0 0 1]); 
f.ospray.setParameter(l0.LightID, f.ospray.OSPRAY_LIGHT, 'direction', [0 1./sqrt(2) -1./sqrt(2)]); 

p.FaceColor = [0.3 0.3 2];
N = per_vertex_normals(V,F);
p.VertexNormals = N;
p.Material = 'metal';
f.ospray.setParameter(0, f.ospray.OSPRAY_MATERIAL, 'roughness', 0.5); 
f.ospray.setParameter(0, f.ospray.OSPRAY_MATERIAL, 'specular', 0.8); 
f.ospray.setParameter(0, f.ospray.OSPRAY_MATERIAL, 'metallic', 0.5); 
f.ospray.setParameter(0, f.ospray.OSPRAY_MATERIAL, 'sheen', 0.1); 


f.Position = [f.Position(1), f.Position(2), 1280, 1024];
vobj = [];
for ii=linspace(0, 2*pi, 50)
    disp(ii)
    f.ospray.setParameter(0, f.ospray.OSPRAY_LIGHT, 'direction', [1*cos(ii)-0.5 -1*sin(ii)-0.5 -1]); 
    drawnowOspray(f);
    
    vobj = figmp4('./ospray_denoiser.mp4',vobj);
end

vobj.close();
