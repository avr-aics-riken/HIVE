print('---This is solversample.lua---')

local names = getMemoryDataNames()

-- dump
if names then
	print('All memory data')
	for i,v in pairs(names) do
		print ('['..i..']=', v)
	end
end



local name = 'testdata'
local data = getMemoryData(name)

-- dump
print(name, "=", data)
if data then	
	for i,v in pairs(data) do
		print (i, v)
	end
end


local loader = ImageLoader()
loader:LoadRawFromPointer(1024,1024,4,8,data.pointer)

local img = loader:ImageData()



local gen  = PrimitiveGenerator()
local plane = gen:Quad(100, 100)

local pm = PolygonModel();
pm:SetShader("textured.frag");
pm:SetTexture('mytex0', img) -- Bind
pm:Create(plane);

local cam = Camera()
cam:SetScreenSize(512, 512)
cam:SetFilename("solversampleout.jpg")
cam:LookAt(
	0,0,300,
	0,0,0,
	0,1,0,
	60
)

render({cam, pm})