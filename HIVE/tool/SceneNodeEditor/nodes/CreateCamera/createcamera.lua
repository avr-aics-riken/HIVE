function CreateCamera()
	print('create camera')
	local cam = Camera()
	cam:LookAt(
		0,0,300,
		0,0,0,
		0,1,0,
		60
	)
	return cam;
end
