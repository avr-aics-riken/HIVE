function RenderWithSIP(arg)
	render(arg.RenderObject)
	for i,v in pairs(arg.RenderObject) do
		print('AAA',i,v)
		if v:GetType() == 'Camera' then
			-- save render image as jpg on memory
			local saver = ImageSaver()
			local imageBuffer = saver:SaveMemory(1, v:GetImageBuffer())
			local imageBufferSize = saver:MemorySize()

			-- create metabinary
			local metabin = MetaBinary()
			local json = [[{
				 "command" : "reqAddContent",
				 "type" : "image",
				 "posx" : "100",
				 "posy" : "100",
				 "width" : "512",
				 "height" : "512"}
			 ]]
			metabin:Create(json, imageBuffer, imageBufferSize) 

			-- send through websockt
			local network = Connection()
			network:Connect('ws://localhost:8082/v1/')
			network:SendBinary(metabin:BinaryBuffer(), metabin:BinaryBufferSize())
			network:Close()
		end
	end
end