import Dispatcher from "./HiveDispatcher.jsx";

export default class HiveAction {
	constructor(id) {
		this.dispatcher = Dispatcher;
		this.id = id;
	}
}
