
CreateCamera = {}
setmetatable(CreateCamera, {__index = BaseComponent})

CreateCamera.new = function (varname)
    local this = BaseComponent.new(varname)
    this.cam = Camera()
    this.property = {
        screensize = {512, 512},
        position = {0,0,300},
        target = {0,0,0},
        up = {0,1,0},
        fov = 60,
        clearcolor = {0,0,0,1},
        color_file = "output.jpg",
        depth_file = ""
    }
    setmetatable(this, {__index=CreateCamera})
    return this
end

function CreateCamera:Do()
    self:UpdateValue()
    local v = self.value
 
    self.cam:SetScreenSize(v.screensize[1], v.screensize[2])
    self.cam:SetFilename(v.color_file)
    self.cam:SetDepthFilename(v.depth_file)
    self.cam:ClearColor(v.clearcolor[1], v.clearcolor[2], v.clearcolor[3], v.clearcolor[4])
    self.cam:LookAt(
        v.position[1], v.position[2], v.position[3],
        v.target[1],   v.target[2],   v.target[3],
        v.up[1],       v.up[2],       v.up[3],
        v.fov
    ) 
end

function CreateCamera:camera()
    return self.cam;
end
