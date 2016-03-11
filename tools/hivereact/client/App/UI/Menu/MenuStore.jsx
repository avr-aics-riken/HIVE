import EventEmitter from 'eventemitter3';
import Core from '../../Core';

export default class MenuStore extends EventEmitter {
    constructor(dispathcer, coreStore, layoutType) {
        super();
        this.dispatchToken = dispathcer.register(this.actionHandler.bind(this));

        this.visibility = (layoutType === 1) || (layoutType === 3) ? true : false;

        // coreStore.on(Core.Constants.NODE_COUNT_CHANGED, (err, data) => {
        // 	this.nodeMap = {};
        // 	for (let i = 0, size = coreStore.getNodes().length; i < size; i = i + 1) {
        // 		this.nodeMap[coreStore.getNodes()[i].varname] = coreStore.getNodes()[i];
        // 	}
        // });

        this.getVisible = this.getVisible.bind(this);
        this.toggleMenu = this.toggleMenu.bind(this);
    }

    actionHandler(payload){
        if (payload && this.hasOwnProperty(payload.actionType)) {
            if (payload.hasOwnProperty("id") && payload.id === this.dispatchToken) {
                (() => {
                    this[payload.actionType].bind(this)(payload);
                })();
            }
        }
    }
    toggleMenu(payload){
        this.visibility = payload.visibility;
        this.emit(MenuStore.TOGGLE_MENU, null, this.visibility);
    }
    getVisible(){
        return this.visibility;
    }
    getDispatchToken() {
        return this.dispatchToken;
    }
}
MenuStore.TOGGLE_MENU = "toggle_menu";
