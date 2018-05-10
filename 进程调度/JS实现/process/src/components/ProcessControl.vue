<template>
  <div>
    <h4 class="is-size-4">{{ type }}</h4>
    <div class="item-list">
      <div v-for="(processItem, index) in this.processArr" :key="processItem.roundCount">
        <h5 class="is-size-5">第{{ index }}轮</h5>
        <ProcessItem :data=processItem />
      </div>
    </div>
  </div>
</template>

<script>
import ProcessItem from "./ProcessItem";

export default {
  components: {
    ProcessItem
  },
  data() {
    return {
      processArr: [],
      type: ""
    };
  },
  methods: {
    /**
     * 将data放入processArr中
     * 
     * @data {Process} (Process的实例对象) 
     * @clearBool {Bool} (用于判断是否重置processArr)
     */
    pushInArr(data, clearBool) {
      let processTurn = data.map(item => item.toObj());
      if (clearBool) {
        this.processArr = [
          {
            ...processTurn
          }
        ];
      } else {
        this.processArr.push(processTurn);
      }
    },
    /**
     * 初始化组件内容
     * 
     * @data {Process} (Process的实例对象) 
     * @type {String} (用于标示调度方法)
     */
    init(data, type) {
      // 初始化优先级
      this.initPriority(data);

      // 初始化0轮
      this.pushInArr(data, true);

      // 分类 执行
      this.switchType(data, type);
    },
    /**
     * 初始化所有进程的优先级，按照需要的CPU时间，逆序赋值
     * 
     * @data {Process} (Process的实例对象) 
     */
    initPriority(data) {
      data.map(item => {
        item.setPriority(50 - item.runTime);
      });
    },
    /**
     * 根据调度种类执行相应的处理函数
     * 
     * @data {Process} (Process的实例对象) 
     * @type {String} (用于标示调度方法)
     */
    switchType(data, type) {
      switch (type) {
        case "static":
          this.staticProcess(data);
          this.type = "静态优先级调度";
          break;
        case "dynamic":
          this.dynamicProcess(data);
          this.type = "动态优先级调度";
          break;
        case "round":
          this.roundProcess(data);
          this.type = "时间片轮转调度";
          break;
        default:
          break;
      }
    },
    /**
     * 静态优先级调度
     * 
     * @data {Process} (Process的实例对象) 
     */
    staticProcess(data) {
      // 判断是否调度完毕
      let sum = 0,
        maxPriorityIndex = -1,
        maxPriorityValue = -1;
      data.map((item, index) => {
        if (item.runTime == 0) {
          sum++;
        } else if (item.priority > maxPriorityValue) {
          maxPriorityIndex = index;
          maxPriorityValue = item.priority;
        }
      });
      if (sum == data.length) {
        // 静态调度完毕
        return;
      } else {
        // 选择调度进程
        data[maxPriorityIndex].state = "Running";
        data[maxPriorityIndex].runTime--;
        data[maxPriorityIndex].cpuTime++;
        if (data[maxPriorityIndex].runTime == 0) {
          data[maxPriorityIndex].state = "Finish";
        }
        this.pushInArr(data);

        this.staticProcess(data);
      }
    },
    /**
     * 动态优先级调度
     * 
     * @data {Process} (Process的实例对象) 
     */
    dynamicProcess(data) {
      // 判断是否调度完毕
      let sum = 0,
        maxPriorityIndex = -1,
        maxPriorityValue = -1;
      data.map((item, index) => {
        if (data[index].state != "Finish") {
          data[index].state = "Waiting";
        }
        if (item.runTime == 0) {
          sum++;
        } else if (item.priority > maxPriorityValue) {
          maxPriorityIndex = index;
          maxPriorityValue = item.priority;
        }
      });
      if (sum == data.length) {
        // 动态调度完毕
        return;
      } else {
        // 选择调度进程
        data[maxPriorityIndex].state = "Running";
        data[maxPriorityIndex].runTime--;
        data[maxPriorityIndex].cpuTime++;
        data[maxPriorityIndex].priority -= 3;
        if (data[maxPriorityIndex].runTime == 0) {
          data[maxPriorityIndex].state = "Finish";
        }
        this.pushInArr(data);

        this.dynamicProcess(data);
      }
    },
    /**
     * 时间片轮转调度
     * 
     * @data {Process} (Process的实例对象) 
     */
    roundProcess(data) {
      // 判断是否调度完毕
        let hasRunIndexArr = [];
      // 轮转
      for (let i = 0; i < data.length; i++) {
        let maxPriorityValue = -1,
          maxPriorityIndex = -1,
          sum = 0

        data.map((item, index) => {
          if (data[index].state != "Finish") {
            data[index].state = "Waiting";
          } else {
            // 剔除空转的进程
            i++;
          }
          if (item.runTime == 0) {
            sum++;
          } else if (
            item.priority > maxPriorityValue &&
            hasRunIndexArr.indexOf(index) == -1
          ) {
            maxPriorityIndex = index;
            maxPriorityValue = item.priority;
          }
        });

        if (sum == data.length) {
          // 时间片轮转调度完毕
          return;
        } else {
          // 轮转过一次
          hasRunIndexArr.push(maxPriorityIndex);

          // 选择调度进程
          data[maxPriorityIndex].state = "Running";
          data[maxPriorityIndex].runTime--;
          data[maxPriorityIndex].allocateRound++;
          if (data[maxPriorityIndex].runTime == 0) {
            data[maxPriorityIndex].state = "Finish";
          }
          this.pushInArr(data);
        }
      }
      this.roundProcess(data);
    }
  }
};
</script>