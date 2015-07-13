function RenderToConsole(arg)
	render(arg.RenderObject)
	for i,v in pairs(arg.RenderObject) do
		print(i, v:GetType())
		if v:GetType() == 'CAMERA' then
			-- save render image as jpg on memory
			local saver = ImageSaver()
			local imageBuffer = saver:SaveMemory(1, v:GetImageBuffer())
			local imageBufferSize = saver:MemorySize()

			print('Send:', 'ws://localhost:58432');
			local network = Connection()
			network:Connect('ws://localhost:58432')
			network:SendBinary(imageBuffer, imageBufferSize)
			network:Close()
			
		end
	end
end
