--[[
	hrender thumbnail capture for HIVE
--]]

local fragpath = "polygon.frag"
local jsonstr = ""
local outputpath = "out.jpg"

for i = 1, #arg do
	if i == 1 then
		fragpath = arg[i]
	end
	if i == 2 then
		outputpath = arg[i]
	end
	if i == 3 then
		jsonstr = arg[i]
	end
end

JSON = require('dkjson')

------------------------------------

function makePolygon()
	local gen = PrimitiveGenerator()
	local meshdata = gen:Teapot(15.0)
	model = PolygonModel()
	model:Create(meshdata)
	model:SetShader(fragpath)
	return model
end

function makeVolume()
	local gentex = GenTexture()
	local size = 64
	local textable = {}
	for k = 1, size do 
		for j = 1, size do 
			for i = 1, size do 
				table.insert(textable, bit32.bxor(k, bit32.bxor(j, i)) / 8 );
			end
		end
	end
	gentex:Create3D(textable, 2, size, size, size);
	local volume = VolumeModel()
	local volumedata = gentex:VolumeData()
	volume:Create(volumedata)
	volume:SetShader(fragpath)
	return volume
end

function makePoint()
	return nil
end

function makeLine()
	return nil
end

function makeTetra()
	return nil
end

------------------------------------

-- Global Jpeg Saver
local saver = ImageSaver()

local camera = Camera()
camera:SetScreenSize(256,256)
camera:LookAt(0,50,90, 2,15,0, 0,1,0, 60)
camera:SetFilename(outputpath)


local model

-- set uniform values
if jsonstr then
	local data
	local pos
	local msg
	data, pos, msg = JSON.decode(jsonstr)
	if not data then
		print("JSON parse error")
		print(pos)
		print(msg)
	end
	if data.type then
		local modeltype = data.type
		if modeltype == 'POLYGON' then
			model = makePolygon()
		elseif modeltype == 'VOLUME' then
			model = makeVolume()
			camera:LookAt(60,50,70, 0,-5,0, 0,1,0, 60)
		elseif modeltype == 'POINT' then
			model = makePoint()
		elseif modeltype == 'LINE' then
			model = makeLine()
		elseif modeltype == 'TETRA' then
			model = makeTetra()
		end
	end
	if data.uniforms then
		local uniforms = data.uniforms
		for key, value in pairs(uniforms) do
			local name, uniform, val
			for k, v in pairs(value) do
				if k == "name" then
					name = v
				end
				if k == "val" then
					val = v
				end
				if k == "uniform" then
					uniform = v
				end
			end
			if name and val and model then
				if uniform == "float" then
					model:SetFloat(name, val)
				end
				if uniform == "vec2" then
					model:SetVec2(name, val[1], val[2])
				end
				if uniform == "vec3" then
					model:SetVec3(name, val[1], val[2], val[3])
				end
				if uniform == "vec4" then
					model:SetVec4(name, val[1], val[2], val[3], val[4])
				end
			end
		end
	end
end

render {camera, model}
