function CreateCamera()
	local cam;
	function create()
		print('create camera')
		cam = Camera()
		cam:LookAt(
			0,0,300,
			0,0,0,
			0,1,0,
			60
		)
		return cam;
	end
	function camera()
		return cam
	end
	return {
		create = create,
		camera = camera
	}
end
