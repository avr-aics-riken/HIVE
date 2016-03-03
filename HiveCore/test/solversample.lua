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


local img = ImageLoader()
img:LoadRawFromPointer(1024,1024,4,8,data.pointer)