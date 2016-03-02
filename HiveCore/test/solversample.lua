print('---This is solversample.lua---')

local name = 'testdata'
local data = getMemoryData(name)
print(name, "=", data)
if data then	
	for i,v in pairs(data) do
		print (i, v)
	end
end


local img = ImageLoader()
img:LoadRawFromPointer(1024,1024,4,8,data.pointer)