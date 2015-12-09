
function RenderToConsole(farg)
	render(farg.RenderObject)
	local mode = false
	local a
	local b
	if arg then
		for a,b in pairs(arg) do
			if b == "MODE=EDITOR" then
				mode = true
			end
		end
	end
	for i,v in pairs(farg.RenderObject) do
		print(i, v:GetType())
		if v:GetType() == 'CAMERA' and mode then
			-- save render image as jpg on memory
			local saver = ImageSaver()
			local imageBuffer = saver:SaveMemory(1, v:GetImageBuffer())
			local imageBufferSize = saver:MemorySize()

			print('Send:', 'ws://localhost:58432');
			local network = require("Network").Connection()
			network:Connect('ws://localhost:58432')
			network:SendBinary(imageBuffer, imageBufferSize)
			network:Close()
			
		end
	end
end
