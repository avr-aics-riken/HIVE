
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
    local property = self.property
    
    local screensize = property.screensize
    local clearcolor = property.clearcolor
    local color_file = property.color_file
    local depth_file = property.depth_file
    local position   = property.position
    local target     = property.target
    local up         = property.up
    local fov        = property.fov
    if self.connection.screensize then
        screensize = self.connection.screensize
    end
    if self.connection.clearcolor then
        clearcolor = self.connection.clearcolor
    end
    if self.connection.color_file then
        color_file = self.connection.color_file
    end
    if self.connection.depth_file then
        depth_file = self.connection.depth_file
    end
    if self.connection.position then
        position = self.connection.position
    end
    if self.connection.target then
        target = self.connection.target
    end
    if self.connection.up then
        up = self.connection.up
    end
    if self.connection.fov then
        fov = self.connection.fov
    end

 
    self.cam:SetScreenSize(screensize[1], screensize[2])
    self.cam:SetFilename(color_file)
    self.cam:SetDepthFilename(depth_file)
    self.cam:ClearColor(clearcolor[1], clearcolor[2], clearcolor[3], clearcolor[4])
    self.cam:LookAt(
        position[1], position[2], position[3],
        target[1],   target[2],   target[3],
        up[1],       up[2],       up[3],
        fov
    ) 
end

function CreateCamera:camera()
    return self.cam;
end
