
export default class MenuAction {
	constructor(dispatcher, id) {
		this.dispatcher = dispatcher;
		this.id = id;
	}

	toggleMenu(visibility) {
		this.dispatcher.dispatch({
			id: this.id,
			actionType: "toggleMenu",
			visibility: visibility
		});
	}

}
