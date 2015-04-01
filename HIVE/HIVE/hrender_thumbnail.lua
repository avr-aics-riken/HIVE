--[[
	hrender thumbnail capture for HIVE
--]]

local objpath = "bunny.obj"
local fragpath = "polygon.frag"
local jsonstr = ""
local outputpath = "out.jpg"

for i = 1, #arg do
	if i == 1 then
		objpath = arg[i]
	end
	if i == 2 then
		fragpath = arg[i]
	end
	if i == 3 then
		outputpath = arg[i]
	end
	if i == 4 then
		jsonstr = arg[i]
	end
end

--[[
JSON = require('dkjson')
local data, pos, msg = JSON.decode(jsonstr)

if data.uniforms then
	local uniforms = data.uniforms
	for key, value in pairs(uniforms) do
		local name, val
		for k, v in pairs(value) do
			print("k")
			print(k)
			print("v")
			print(v)
		end
	end
end
--]]

-- Global Jpeg Saver
local saver = ImageSaver()

local camera = Camera()
camera:SetScreenSize(256,256)
camera:LookAt(0,0,100, 0,0,0, 0,1,0, 60)
camera:SetFilename(outputpath)

local obj = OBJLoader()
obj:Load(objpath)
local model = PolygonModel()
local meshdata = obj:MeshData()
model:Create(meshdata)
model:SetShader(fragpath)
-- TODO
--model:SetVec4('color', 1.0, 1.0, 1.0, 1.0);
render {camera, model}

