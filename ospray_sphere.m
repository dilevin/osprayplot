%load data
[Vs, Fs] = readOBJ('/Users/dilevin/Downloads/sphere-stripe.obj');

%start ospray and add geometry
f = figure; p = ospraytsurf(Fs,Vs);
hold on; p1 = ospraytsurf([1 2 3; 1 3 4], [-100 -100 -1; 100 -100 -1; 100 100 -1; -100 100 -1]);

p.FaceColor = [1 1 1];
p1.FaceColor = [1.2 1.2 1.2];

%turn on denoiser
f.ospray.Denoise = 1;
f.ospray.SamplesPerPixel = 4;

%add lights
l0 = ospraylight('ambient');
l1 = ospraylight('sunSky');
f.ospray.setParameter(l0.LightID, f.ospray.OSPRAY_LIGHT, 'intensity', 0.8); 
f.ospray.setParameter(l1.LightID, f.ospray.OSPRAY_LIGHT, 'intensity', 0.75); 
f.ospray.setParameter(l1.LightID, f.ospray.OSPRAY_LIGHT, 'up', [0 0 1]); 
f.ospray.setParameter(l1.LightID, f.ospray.OSPRAY_LIGHT, 'direction', [-1 2 -1]); 

%normals and texture coordinates
N = per_vertex_normals(Vs,Fs);
p.VertexNormals = N;
p.TextureCoords = [0.5*ones(size(Vs,1),1), (Vs(:,1)-min(Vs(:,1)))/(max(Vs(:,1))-min(Vs(:,1)))+0.5/1024];

%color map
b = cbrewer('Oranges', 8);
b1 = reshape(repmat(b(:,1)',128,1), 1024, 1);
b2 = reshape(repmat(b(:,2)',128,1), 1024, 1);
b3 = reshape(repmat(b(:,3)',128,1), 1024, 1);

color_table = zeros(size(b1,1), 1, 3);
color_table(:,1,1) = b1;
color_table(:,1,2) = b2;
color_table(:,1,3) = b3;

%load texture
t0 = ospraytexture(color_table);
f.ospray.setParameter(0, f.ospray.OSPRAY_MATERIAL, 'map_kd', t0); 
drawnowOspray(f);