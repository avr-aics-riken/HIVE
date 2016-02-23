
function RenderView(arg)
	local cam;	
	print('create camera',
		property.screensize[1], property.screensize[2],
		property.filename)
	cam = Camera()
	cam:SetScreenSize(property.screensize[1], property.screensize[2])
	cam:SetFilename(property.color_file)
	if property.depth_file ~= nil then
		cam:SetDepthFilename(property.depth_file)
	end
	cam:ClearColor(property.clearcolor[1],property.clearcolor[2],property.clearcolor[3],property.clearcolor[4])
	cam:LookAt(
		property.position[1], property.position[2], property.position[3],
		property.target[1], property.target[2], property.target[3],
		property.up[1], property.up[2], property.up[3],
		property.fov
	)
	
	arg.RenderObject[#arg.RenderObject + 1] = cam; 
 
	render(arg.RenderObject)
end