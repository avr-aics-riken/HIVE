function CreateCamera(property)
	local cam;	
	print('create camera',
		property.screensize[1], property.screensize[2],
		property.filename)
	cam = Camera()
	cam:SetScreenSize(property.screensize[1], property.screensize[2])
	cam:SetFilename(property.filename)
	cam:LookAt(
		property.position[1], property.position[2], property.position[3],
		property.target[1], property.target[2], property.target[3],
		property.up[1], property.up[2], property.up[3],
		property.fov
	)
	
	function camera()
		return cam
	end
	return {
		camera = camera
	}
end
