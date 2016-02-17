import Dispatcher from "./Dispatcher.jsx";

export default class HiveAction {
	constructor(id) {
		this.dispatcher = Dispatcher;
		this.id = id;
	}
}
