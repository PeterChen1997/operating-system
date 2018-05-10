// PCB的模拟类
export default class Process {
  constructor(name, runTime) {
    this.name = name
    this.runTime = runTime
    this.priority = 1
    this.allocateRound = 0
    this.cpuTime = 0
    this.state = 'Waiting'
    this.count = 0
  }

  setPriority(value) {
    this.priority = value
  }

  toObj() {
    return {
      name: this.name,
      runTime: this.runTime,
      priority: this.priority,
      allocateRound: this.allocateRound,
      cpuTime: this.cpuTime,
      state: this.state,
      count: this.count
    }
  }
}