

export class NodeIterator {
	constructor() {
	}
}

/**
 * あるノードの入力ノードを列挙するイテレータ.
 * arrayがあった場合は、arrayの中身を辿る.
 */
NodeIterator.makeInputIterator = (store, node) => {
	return {
		[Symbol.iterator]() {
			let currentNode = node;
			let inputIndex = 0;
			let arrayIndex = 0;
			return {
				next() {
					let input = null;
					if (inputIndex < currentNode.input.length) {
						input = currentNode.input[inputIndex];
						if (Array.isArray(input.array)) {
							if (arrayIndex < input.array.length) {
								input = input.array[arrayIndex];
								++arrayIndex;
							} else {
								input = currentNode.input[++inputIndex];
								arrayIndex = 0;
							}
						} else {
							++inputIndex;
							arrayIndex = 0;
						}

						return {
							value : {
								input : input
							},
							done : false
						};
					} else {
						return {
							value : {
								input : null
							},
							done : true
						};
					}
				}
			};
		}
	}
};


export default new NodeIterator();
