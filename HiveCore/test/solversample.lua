--
-- Setup: Copy def_volume_raymarch_tf.frag from $HIVE/hrender/test
--
print('---This is solversample.lua---')

local names = getMemoryDataNames()

-- dump
if names then
	print('All memory data')
	for i,v in pairs(names) do
		print ('['..i..']=', v)
	end
end



local name = 'SparseVolumeScalar'
local data = getMemoryData(name)

-- dump
print(name, "=", data)
if data then	
	for i,v in pairs(data) do
		print (i, v)
	end
end


local sparseVol = require("RawSparseVolumeLoader")()
sparseVol:LoadFromPointer(data.pointer)

local volume = SparseVolumeModel()
volume:Create(sparseVol:SparseVolumeData())

local camera = Camera()
camera:SetScreenSize(1024, 1024)
camera:SetFilename('image_solversample.jpg')

camera:LookAt(
   200,200,200,
     0,0,0,
     0,1,0,
     60)

function JetTransferFunctionTexture()
   -- Create 1D transfer function texture
   -- Jet color table from OSPray example
   -- r, g, b, alpha
   local tf = 
        { 0         , 0           , 0.562493 , 1.0
        , 0         , 0           , 1        , 1.0
        , 0         , 1           , 1        , 1.0
        , 0.500008  , 1           , 0.500008 , 1.0
        , 1         , 1           , 0        , 1.0
        , 1         , 0           , 0        , 1.0
        , 0.500008  , 0           , 0        , 1.0 }

   local width = #tf / 4
   local tfTex = GenTexture()
   tfTex:Create2D(tf, 3, width, 1); -- 3 = RGBA32F

   return tfTex;
end

local tftex = JetTransferFunctionTexture()
volume:SetShader('def_volume_raymarch_tf.frag') -- $HIVE/hrender/test
volume:SetTexture('tf_tex', tftex:ImageData())
volume:SetClampToEdge(true, true, true)
volume:SetTextureWrapping('tf_tex', true, true, true) -- CLAMP_TO_EDGE wrap mode for transfer function texture.
volume:SetFloat('tf_min', 0.0)
volume:SetFloat('tf_max', 1.0)
volume:SetFloat('tf_opacity', 0.0025)

render {camera, volume}
