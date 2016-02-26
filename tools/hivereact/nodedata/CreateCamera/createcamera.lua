
CreateCamera = {}

CreateCamera.new = function ()
    local this = {}
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

function CreateCamera:Set(propname, value)
    self.property[propname] = value
    self.updated = true;
end

function CreateCamera:Do()
    local property = self.property    
    self.cam:SetScreenSize(property.screensize[1], property.screensize[2])
    self.cam:SetFilename(property.color_file)
    if property.depth_file ~= nil then
        self.cam:SetDepthFilename(property.depth_file)
    end
    self.cam:ClearColor(property.clearcolor[1],property.clearcolor[2],property.clearcolor[3],property.clearcolor[4])
    self.cam:LookAt(
        property.position[1], property.position[2], property.position[3],
        property.target[1], property.target[2], property.target[3],
        property.up[1], property.up[2], property.up[3],
        property.fov
    ) 
end

function CreateCamera:camera()
    return self.cam;
end
