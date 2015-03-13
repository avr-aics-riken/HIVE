

local function ClearObjects()
	HIVE_ObjectTable = {}
	clearCache()
	collectgarbage('collect')
	print('MEMORY=', collectgarbage('count') .. '[KB]')
	return 'ClearObject'
end
	
local function CreateCamera(name)
	local camera = Camera()
	camera:SetScreenSize(512, 512)
	camera:LookAt(
		0,0,300,
		0,0,0,
		0,1,0,
		60
	)
	HIVE_ObjectTable[name] = {camera}
	print('ADD CAEMRA: '.. name)
	return 'CreateCamera:' .. name
end

local function NewScene()
	ClearObjects()
	CreateCamera('view')
	return 'NewScene!'
end

-- TEST METHOD
local function CameraPos(name, camerapos_x, camerapos_y, camerapos_z)
	local camera = HIVE_ObjectTable[name]
	if camera == nil then return 'Not found camera' end
	camera[1]:LookAt(
		camerapos_x, camerapos_y, camerapos_z,
		0,0,0,
		0,1,0,
		60
	)
end

local function CameraScreenSize(name, width, height)
	local camera = HIVE_ObjectTable[name]
	if camera == nil then return 'Not found camera' end
	camera[1]:SetScreenSize(width, height)
	return 'ScreenSize:' .. name
end

local function GetObjectList()
	local lst = {}
	for i,v in pairs(HIVE_ObjectTable) do
		lst[#lst + 1] = {name = i, type = v[1]:GetType()}
	end
	return lst
end
	
local function LoadSPH(name, filename, shader)
	local sph = SPHLoader()
	local ret = sph:Load(filename)
	if ret then
	    local volumemodel = VolumeModel()
	    local volume = sph:VolumeData()
	    volumemodel:Create(volume)
	    volumemodel:SetShader(shader)
		HIVE_ObjectTable[name] = {volumemodel}
	end
	return 'LoadPDB:' .. tostring(ret)
end

local function LoadOBJ(name, filename, shader)
	local obj = OBJLoader()
	local ret = obj:Load(filename)
	if ret then
		local model = PolygonModel()
		local meshdata = obj:MeshData()
		model:Create(meshdata)
		model:SetShader(shader)
		HIVE_ObjectTable[name] = {model}
	end
	return 'LoadOBJ:' .. tostring(ret)
end


local function LoadSTL(name, filename, shader)
	local stl = STLLoader()
	local ret = stl:Load(filename)
	if ret then
		local model = PolygonModel()
		local meshdata = stl:MeshData()
		model:Create(meshdata)
		model:SetShader(shader)
		HIVE_ObjectTable[name] = {model}
	end
	return 'LoadSTL:' .. tostring(ret)
end

local function LoadPDB(name, filename, shader)
	local pdb = PDBLoader()
	local ret = pdb:Load(filename)
	if ret then
	    local ballmodel = PointModel()
	    local balldata = pdb:BallData()
	    ballmodel:Create(balldata)
	    ballmodel:SetShader(shader)
	    local stickmodel = LineModel()
	    local stickdata = pdb:StickData()
	    stickmodel:Create(stickdata)
	    stickmodel:SetLineWidth(0.20);
	    stickmodel:SetShader(shader)
		HIVE_ObjectTable[name] = {ballmodel, stickmodel}
	end
	return 'LoadPDB:' .. tostring(ret)
end
local function RenderCamera(w, h, cameraname)
	--print('RenderCamera', w, h, cameraname)
	local starttm = os.clock()
	local camera = HIVE_ObjectTable[cameraname]
	if camera == nil then return "No Camera" end
	camera = camera[1] -- one object
	camera:SetScreenSize(w, h)
	local renderList = {camera}
	for i,v in pairs(HIVE_ObjectTable) do
		print ("OBJECT = ", i, v)
		for i2,v2 in pairs(v) do
			print ("RENDEROBJE=", i2, v2)
		    if v2:GetType() ~= "CAMERA" then
		        renderList[#renderList + 1] = v2
		    end
	   	end
	end
	--print('RenderObject = ', #renderList)
	local r = render(renderList, HIVE_fetchEvent)
	--print('Render Ret = ', r)
	
	-- save jpg
	local rendertm = os.clock()
	local imageBuffer = HIVE_ImageSaver:SaveMemory(1, camera:GetImageBuffer())
	local imageBufferSize = HIVE_ImageSaver:MemorySize()
	local savetm = os.clock()
	-- make metabin
	local json = [[{
		"JSONRPC": "2.0",
		"method" : "renderedImage",
		"param" : {
		 	"type": "jpg",
		    "width" : ]] .. camera:GetScreenWidth() .. [[,
			"height" : ]] .. camera:GetScreenHeight() .. [[,
		 	"canceled": ]] .. tostring(HIVE_isRenderCanceled) .. [[
		},
		"to": "client",
		"id": 0
	}]]
	HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
	local createtm = os.clock()
	-- send through websocket
	network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
	local sendtm = os.clock()
	print("render=", rendertm-starttm, "save=", savetm-rendertm,"createmeta=", createtm-savetm, "send=", sendtm-createtm, "all=", sendtm-starttm)

	collectgarbage('collect') -- NEED for GC
	return {objectnum=tostring(r), width=camera:GetScreenWidth(), height=camera:GetScreenHeight()}
end


return {
	NewScene = NewScene,
	ClearObjects = ClearObjects,
	CreateCamera = CreateCamera,
	CameraPos = CameraPos,
	CameraScreenSize = CameraScreenSize,
	GetObjectList = GetObjectList,
	LoadSPH = LoadSPH,
	LoadOBJ = LoadOBJ,
	LoadSTL = LoadSTL,
	LoadPDB = LoadPDB,
	RenderCamera = RenderCamera,
}